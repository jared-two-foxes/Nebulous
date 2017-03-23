
#include "Component.h"
#include "ComponentCache.h"

#include <Nebulae/Common/Logger/Logger.h>

using namespace Game;
using namespace Nebulae;

ComponentCache::ComponentCache()
///
/// Constructor
///
{}

ComponentCache::~ComponentCache()
///
/// Destructor
///
{
  std::for_each( m_pools.begin(), m_pools.end(), [](ComponentPool* pool){ delete pool; });
}

void 
ComponentCache::Clear()
///
/// Clear's all of the ComponentPool objects that are currently owned.
///
/// @return
///   Nothing.
///
{
  std::for_each( m_pools.begin(), m_pools.end(), [](ComponentPool* pool) { pool->Clear(); });
}

void 
ComponentCache::AddComponentPool( ComponentPool* pool )
///
/// Add's a ComponentPool object to the list of owned pools.
///
/// @return
///   Nothing.
///
{
  m_pools.push_back( pool ); 
}

void
ComponentCache::RemoveComponentPool( ComponentPool* pool )
///
/// Removes a ComponentPool object from the list of owned pools.
///
/// @return
///   Nothing.
///
{
  
}

int32 
ComponentCache::GetNextAvailableIndex( ComponentType type ) const
///
/// Get's the identifier of the next available Component of the given type.
///
/// @param type
///   The ComponentType to query
///
/// @return
///   The identifier if it could be found, else Comonent::UNUSED will be 
///   returned.
///
{
  ComponentPool* pool = GetComponentPool( type );
  if( pool != NULL )
  {
    return pool->GetNextAvailableIndex();
  }

  return Component::UNUSED; 
}

void  
ComponentCache::AllocateComponent( ComponentType type, int32 component, int32 allocatedTo )
///
/// Operates like an aquire style function, associating a Component object [type,component]
/// with an object represnted by allocatedTo which is typically an entity identifier.
///
/// @param type
///   The ComponentType value.
///
/// @param component
///   The Component identifier.
///
/// @param allocatedTo
///   The object that the component is to be associated with.
///
/// @return
///   Nothing.
///
{
  ComponentPool* pool = GetComponentPool( type );
  if( pool != NULL )
  {
    pool->AllocateComponent( component, allocatedTo );
  }
}

bool  
ComponentCache::ReleaseComponent( ComponentType type, int32 component, int32 deallocatedFrom )
///
/// The Release function in an aquire/release pair.  Removes the association of a Component 
/// object [type,component] with an object represnted by deallocatedFrom which is typically
/// an entity identifier.
///
/// @param type
///   The ComponentType value.
///
/// @param component
///   The Component identifier.
///
/// @param allocatedTo
///   The object that the component is to be associated with.
///
/// @return
///   Nothing.
///
{
  ComponentPool* pool = GetComponentPool( type );
  if( pool != NULL )
  {
    return pool->ReleaseComponent( component, deallocatedFrom );
  }

  NE_LOG( "Unable to find a pool for the specified ComponentType" );
  return false;
}

ComponentPool* 
ComponentCache::GetComponentPool( ComponentType type ) const
///
/// Attempt to find the ComponentPool for the specified ComponentType.
///
/// @param type
///   The ComponentType to query
///
/// @return
///   The ComponentPool if it could be found, else NULL.
///
{
  for( uint32 i = 0, n = m_pools.size(); i < n; ++i )
  {
    if( m_pools[i]->GetType() == type )
    {
      return m_pools[i];
    }
  }

  return NULL;
}