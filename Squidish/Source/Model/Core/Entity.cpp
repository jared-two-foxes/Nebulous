
#include "Entity.h"

using namespace Game;


Entity::Entity( uint32 identifier ) 
: m_identifier(identifier), 
  m_enabled(true) {}

Entity::~Entity() 
{}

int32
Entity::GetIdentifier() const
{ return m_identifier; }

const char*
Entity::GetUniqueName() const
{ return m_uniqueName.c_str(); }

bool
Entity::IsEnabled() const
{ return m_enabled; }

int32
Entity::GetLinkedComponentIndices( ComponentType componentType, std::vector<int32 >* indices ) const
{
  int32 count = 0;

  for( std::vector<ComponentLink >::const_iterator it = m_components.begin(); it != m_components.end(); ++it )
  {
    if( (*it).type == componentType )
    {
      count++;
      
      if( indices != NULL )
      {
        indices->push_back( (*it).identifier );
      }
    }
  }

  return count;
}

void
Entity::SetUniqueName( const char* uniqueName ) 
{ m_uniqueName = uniqueName; }

void
Entity::SetEnabled( bool enabled )
{ m_enabled = enabled; }

bool
Entity::AddComponent( ComponentType componentType, int32 identifier )
{
  //@todo Check that AddComponent doesnt register the same component twice.
  if( !HasComponentLinkage(componentType, identifier) )
  {
    m_components.push_back( ComponentLink() );
    ComponentLink& link = m_components.back();
    link.type           = componentType;
    link.identifier     = identifier;

    return true;
  }

  return false;
}

void
Entity::RemoveComponent( ComponentType componentType, int32 identifier )
{
  for( std::vector<ComponentLink >::const_iterator it = m_components.begin(); it != m_components.end(); ++it )
  {
    if( (*it).type == componentType && (*it).identifier == identifier )
    {
      m_components.erase( it );
      break;
    }
  }
}

bool
Entity::HasComponentLinkage( ComponentType componentType, int32 identifier ) const
{
  for( std::vector<ComponentLink >::const_iterator it = m_components.begin(); it != m_components.end(); ++it )
  {
    if( (*it).type == componentType && (*it).identifier == identifier )
    {
      return true;
    }
  }

  return false;
}
