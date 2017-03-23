#ifndef __NEBULAE_BETA_PROFILE_H__
#define __NEBULAE_BETA_PROFILE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Clock;

void InitProfiler( Clock& systemClock );
void ResetProfiler();
void ProfileBegin( const char* name );
void ProfileEnd( const char* name );
void MarkFrame();

std::size_t WriteProfileDataToBuffer( std::string& output ); //< todo make this take an std::ostream and so that user can pass either cout or a stringstream etc.


class ScopedProfiler
{
private:
  std::string m_name;

public:
  ScopedProfiler( const char* name )
  : m_name(name)
  {
    ProfileBegin( m_name.c_str() );
  }

  ~ScopedProfiler()
  {
    ProfileEnd( m_name.c_str() );
  }

};

#ifdef NE_DEBUG
#define PROFILE               Nebulae::ScopedProfiler scoped_profilier( __FUNCTION__ )
#define NAMED_PROFILE( name ) Nebulae::ScopedProfiler name_scoped_profilier( NE_STRINGIZE(name) )
#else
#define PROFILE
#define NAMED_PROFILE( name ) 
#endif

}

#endif // __NEBULAE_BETA_PROFILE_H__