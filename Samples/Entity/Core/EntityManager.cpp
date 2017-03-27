
#include "EntityManager.h"

#include <Samples/Entity/Core/Entity.h>

namespace Sample
{
  
EntityManager::EntityManager()
///
/// Default Constructor.  Does nothing interesting.
///
{
  
}

EntityManager::~EntityManager()
///
/// Destructor
///
{
  

}

void
EntityManager::RegisterAllocator( int type, Allocator* allocator )
///
/// Associates an Allocator object with an entity type enumeration.
///
/// @param type
///   The enumeration to which we want to assign the allocator too.
///
/// @param allocator
///   The object that we will use to create an entity of type /type
///
/// @return
///   Nothing.
///
{
  assert( m_allocators[type] == NULL && "Allocator already registered" );
  m_allocators[type] = allocator;
}

void 
EntityManager::GetEntities( std::vector<Entity*>* entitiesOut ) const
///
///
///
///
///
///
{
  std::vector<Entity*>::const_iterator end_it = m_entities.end();
  for( std::vector<Entity*>::const_iterator it = m_entities.begin(); it != end_it; ++it )
  {
    entitiesOut->push_back( *it );
  }
}

}