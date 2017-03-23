
#include "EntityTemplate.h"

namespace Game
{

EntityTemplate::EntityTemplate( int id, const char* name, const std::vector<std::string>& components )
///
/// Constructor
///
  : m_id( id )
  , m_name( name )
  , m_components( components )
{
  
}


int 
EntityTemplate::GetId() const
///
/// Gets the system template identifier
///
/// @return
///    The identifier
///
{
  return m_id; 
}
    

const std::string&
EntityTemplate::GetName() const 
///
/// Gets the name of the template.
///
/// @return
///   The name of the template.
///
{
  return m_name; 
}


bool 
EntityTemplate::ContainsComponent( const std::string& component ) const
///
/// Iterates the names of the components held by this entity for a specifically named 
/// component.
///
/// @param component
///   The name of the component to look for.
///
/// @return
///   Whether the component was found.
///
{
  std::vector<std::string>::const_iterator end_it = m_components.end();
  for( std::vector<std::string>::const_iterator it = m_components.begin(); it != end_it; ++it )
  {
    if( (*it) == component )
    {
      return true;
    }
  }
  return false;
}

}