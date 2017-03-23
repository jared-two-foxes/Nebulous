
#include "EntityPool.h"
#include "Entity.h"

namespace Game
{
  
EntityPool::EntityPool()
///
/// Default Constructor.  Does nothing interesting.
///
{
  
}


EntityPool::~EntityPool()
///
/// Destructor
///
{
  while( !m_availableEntities.empty() )
  {
    Entity* obj = m_availableEntities.back();
    delete obj;
    m_availableEntities.pop_back();
  }

  while( !m_usedEntities.empty() )
  {
    Entity* obj = m_usedEntities.back();
    delete obj;
    m_usedEntities.pop_back();
  }
}


void
EntityPool::Init( uint32 entityCount )
///
/// Creates the entities that will exist in this pool and places them in the available list.
///
/// @param entityCount
///   number of entities to create.
///
/// @return 
///   Nothing.
///
{
  for( uint32 i = 0; i < entityCount; ++ i )
  {
    m_availableEntities.push_back( new Entity( i ) );
  }
}


void
EntityPool::Clear()
///
/// Returns all "used" entities back to the available list effectively releaeing all of the
/// currently used entities for reuse.
///
/// @return
///   Nothing.
///
{
  while( !m_usedEntities.empty() )
  {
    Entity* obj = m_usedEntities.back();
    m_availableEntities.push_back( obj );
    m_usedEntities.pop_back();
  }
}


Entity* 
EntityPool::SpawnEntity( const char* uniqueName )
///
/// Creates a new entity.
///
/// @param templateName
///   The identifier of the template to apply to created entity.
///
/// @return
///   The spawned entity.
///
/// @toto [jared.watt 05-10-2013]
///   Should also check length of uniqueIdenfier, ie dont assign an empty or "" string.
///
{
  NE_ASSERT( !m_availableEntities.empty(), "There are no remaining available entities." )();

  if( m_availableEntities.empty() ) {
    return NULL;
  }

  Entity* obj = m_availableEntities.front();
  m_availableEntities.pop_front();
  m_usedEntities.push_back( obj );
  if( uniqueName != NULL ) {
    obj->SetUniqueName( uniqueName );
  }
  return obj;
}


Entity*
EntityPool::FindEntityByUniqueName( const char* uniqueName ) const
{
  std::vector<Entity*>::const_iterator end = m_usedEntities.end();
  for( std::vector<Entity*>::const_iterator iter = m_usedEntities.begin(); iter != end; ++iter ) {
    if( strcmp((*iter)->GetUniqueName(), uniqueName) == 0 ) {
      return (*iter);
    }
  }
  return NULL;
}

Entity*
EntityPool::FindEntityByIdentifier( int32 identifier ) const
{
  std::vector<Entity*>::const_iterator end = m_usedEntities.end();
  for( std::vector<Entity*>::const_iterator iter = m_usedEntities.begin(); iter != end; ++iter ) {
    if( (*iter)->GetIdentifier() == identifier ) {
      return (*iter);
    }
  }
  return NULL;
}

const std::vector<Entity*>&
EntityPool::GetEntities() const
///
/// Retrieves the list of all used entities.
///
/// @return
///   Nothing.
///
{
  return m_usedEntities;
}


}