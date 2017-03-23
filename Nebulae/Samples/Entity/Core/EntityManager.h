#ifndef __CORE_ENTITYMANAGER_H__
#define __CORE_ENTITYMANAGER_H__

#include <Nebulae/Common/Common.h>

namespace Sample
{
  
class Entity;

class EntityManager
///
/// Can be considered the interface to the 'Model' layer of the application.  
///
{
public:
  struct Allocator
  ///
  /// Allocator object used for creating a specific type of Entity.  Derived classes _MUST_
  /// implement the Create function.
  ///
  {
    virtual Entity* Create() const = 0;
  };

private:
  std::vector< Entity* >      m_entities;   ///< A list of all of the Entities in the universe.
  std::map< int, Allocator* > m_allocators; ///< A list of the allocators used to create the various entities.

public:
  EntityManager();
  ~EntityManager();

  void RegisterAllocator( int type, Allocator* allocator );

  template <class T>
  T* SpawnEntity( int type );

  void GetEntities( std::vector<Entity*>* entitiesOut ) const;
  
};

template <class T>
T* 
EntityManager::SpawnEntity( int type )
{
  //todo assert if T is not an Entity type.

  // T::GetType(); ???
  Allocator* allocator = m_allocators[ type ];
  assert( allocator );
  T* obj = static_cast<T*>(allocator->Create());
  m_entities.push_back( obj );
  return obj;
}

}

#endif // __CORE_ENTITYMANAGER_H__
