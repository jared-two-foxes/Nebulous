/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */


#include "Profile.h"

#include <Nebulae/Common/Platform/Clock.h>

using namespace Nebulae;


struct CallSite : public std::vector<std::shared_ptr<CallSite > >
///
/// Defines a list type that contains shared_ptr's to other CallSite objects.  Each object contained
/// within the array can be considered a child of this CallSite.
///
{
public:
  std::string  szName;                ///< Name of sample
  unsigned int iProfileInstances;     ///< # of times ProfileBegin called
  int          iOpenProfiles;         ///< # of times ProfileBegin w/o ProfileEnd
  uint64       startTime;             ///< The current open profile start time
  uint64       accumulator;
  uint64       min;
  uint64       max;

  public:
    CallSite( const std::string& name )
    : szName( name ),
      iOpenProfiles( 0 ),
      iProfileInstances( 0 ),
      min( std::numeric_limits<uint64 >::max() ),
      max( std::numeric_limits<uint64 >::min() ),
      startTime( 0 ),
      accumulator( 0 )
      {}

    std::shared_ptr<CallSite > find( const std::string& name )
    {
      auto find_site_by_name = [&](std::shared_ptr<CallSite > site) { return (site->szName == name); };
      auto itr               = std::find_if( begin(), end(), find_site_by_name );
      
      return itr != end() ? (*itr)->find(name) : nullptr;
    }

};


Clock* g_systemClock = nullptr;
std::vector<std::shared_ptr<CallSite > > m_call_sites;
std::vector<std::shared_ptr<CallSite > > m_call_stack;


struct ProfileWriter 
///
/// todo: Count the root frame?
///
{
  std::size_t count_digits( int32 x )  
  {  
    x = abs( x );  

    return (x < 10 ? 1 :   
    	(x < 100 ? 2 :   
    	(x < 1000 ? 3 :   
    	(x < 10000 ? 4 :   
    	(x < 100000 ? 5 :   
    	(x < 1000000 ? 6 :   
    	(x < 10000000 ? 7 :  
    	(x < 100000000 ? 8 :  
    	(x < 1000000000 ? 9 :  
    	10)))))))));  
  }

  void print_call_site( std::string& output_str, std::shared_ptr<CallSite >& call_site, std::size_t nlength, std::size_t clength, std::size_t depth ) 
  {
    char   line[256];
   
    uint64 aveTime = call_site->accumulator;;
    
    std::string name( call_site->szName );
    for( std::size_t i = 0; i < depth; ++i ) 
    {
      name = "  " + name;
    }

    sprintf( line, "%-*.*s : %*u : %9llu : %9lli : %9lli :\n", static_cast<int>(nlength), static_cast<int>(nlength), name.c_str(), static_cast<int>(clength), 
      call_site->iProfileInstances, aveTime, call_site->min, call_site->max );
    
    output_str += line;	// Send the line to text buffer
  };

  void iterate_children( std::string& output_str, std::shared_ptr<CallSite >& call_site, std::size_t nlength, std::size_t clength, std::size_t depth ) 
  {
    auto sort_children_by_size = []( std::shared_ptr<CallSite >& lhs,  std::shared_ptr<CallSite >& rhs ) {
      uint32 lhs_avg = lhs->max/* / lhs->iProfileInstances*/;
      uint32 rhs_avg = rhs->max/* / rhs->iProfileInstances*/;
      return (rhs_avg < lhs_avg);
    };
 
    std::sort( call_site->begin(), call_site->end(), sort_children_by_size );

    print_call_site( output_str, call_site, nlength, clength, depth );
    
    for( auto child_itr : *call_site ) 
    {
      iterate_children( output_str, child_itr, nlength, clength, depth + 1 );
    }
  }

  void print_tree( std::string& output_str, std::shared_ptr<CallSite >& call_site ) 
  {
    const char* title        = "Profile Name";
    std::size_t name_length  = strlen( title );
    std::size_t count_length = 0;

    auto find_max_indented_name = [&]( std::shared_ptr<CallSite >& site ) {
      name_length = std::max<size_t >( name_length, site->szName.length() );
    };
  
    auto find_max_profile_length = [&]( std::shared_ptr<CallSite >& site ) {
      count_length = std::max<size_t >( count_length, count_digits(site->iProfileInstances) );
    };

    std::for_each( m_call_sites.begin(), m_call_sites.end(), find_max_indented_name );
    std::for_each( m_call_sites.begin(), m_call_sites.end(), find_max_profile_length );

    // Add the title.
    char header[128];
    int length = sprintf_s( header, 128, "%-*s :  #  :  Ave  :   Min   :   Max   :\n", static_cast<int>(name_length), title ); //< make the '#' length the same as clength
    output_str += header;

    std::string str;
    while( length > 0 )
    {
      str += "-";
      --length;
    }
    str += "\n";
    output_str += str;

    iterate_children( output_str, call_site, name_length, count_length, 0 );
  }
};


void 
Nebulae::InitProfiler( Clock& systemClock )
{
  g_systemClock = &systemClock;
  auto root_node = std::make_shared<CallSite >( "root" );
  m_call_sites.push_back( root_node );
  m_call_stack.push_back( root_node );
}

void 
Nebulae::ResetProfiler()
{
  for( std::shared_ptr<CallSite >& sample : m_call_sites )
  {
    sample->iProfileInstances  = 1;
    sample->iOpenProfiles      = 0;
    sample->startTime          = -1; 
    sample->accumulator        = 0;
    sample->min                = std::numeric_limits<uint64 >::max();
    sample->max                = std::numeric_limits<uint64 >::min();
  }
}

void 
Nebulae::ProfileBegin( const char* name )
{
  std::shared_ptr<CallSite > node, parent;
  std::vector<std::shared_ptr<CallSite > >::iterator child_itr;
  
  auto find_call_site_by_name = [&]( std::shared_ptr<CallSite >& call_site ) {
    return ( call_site->szName == name );
  };

  if( !m_call_stack.empty() )
  {
    parent = m_call_stack.back();

    child_itr = std::find_if( parent->begin(), parent->end(), find_call_site_by_name );
  }

  if( parent == nullptr || child_itr == parent->end() )
  {
    node = std::make_shared<CallSite >( name );
    m_call_sites.push_back( node );
    if( parent != nullptr )
    {
      parent->push_back( node );
    }
  }
  else
  {
    node = *child_itr;
  }

  // Found the sample
  node->iOpenProfiles++;
  node->iProfileInstances++;
  node->startTime = g_systemClock->GetAccurateTime();

  m_call_stack.push_back( node );
}

void 
Nebulae::ProfileEnd( const char* name )
{
  NE_ASSERT( !m_call_stack.empty(), "Attempting to end a profile block on an empty queue" )();
  NE_ASSERT( m_call_stack.back()->szName == name, "ProfileBlock at back of stack does not match" )();

  std::shared_ptr<CallSite > node = m_call_stack.back();
  
  // Found the sample;
  node->iOpenProfiles--;

  // Save sample time in accumulator
  uint64 iterationTime = g_systemClock->GetAccurateTime() - node->startTime; 
  node->min            = std::min<uint64 >(node->min, iterationTime);
  node->max            = std::max<uint64 >(node->max, iterationTime);
  node->accumulator   += iterationTime;
  
  m_call_stack.pop_back();
}

void 
Nebulae::MarkFrame()
{
  std::for_each( m_call_sites.begin(), m_call_sites.end(), [&]( const std::shared_ptr<CallSite >& call_site ) {
    call_site->iProfileInstances = 0;
    call_site->accumulator = 0;
  });
}

std::size_t
Nebulae::WriteProfileDataToBuffer( std::string& output_str )
{
  // Add in each profile value.
  NE_ASSERT( !m_call_sites.empty(), "CallSites list should never be empty" )();
  ProfileWriter profiler_writer;
  profiler_writer.print_tree( output_str, m_call_sites[0] );

  output_str += "\n";

  return output_str.length();
}