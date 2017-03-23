#ifndef __MODEL_CORE_COMPONENTCACHE_H__
#define __MODEL_CORE_COMPONENTCACHE_H__

#include <Nebulae/Common/Common.h>

#include "ComponentPool.h"
#include "ComponentType.h"

namespace Sample {

class ComponentCache
///
/// Encapsulates the various different components available to Entities in the game system. 
///
{
private:
  std::vector<ComponentPool* > m_pools;
  
  public:
    ComponentCache();

    void Clear();   
    void Init( int count );
    void AddPoolForComponentType( ComponentType type );
    void RemoveComponentPool( ComponentType type );

    template <class T >
    T* GetComponent( int32 identifier ) const;

    template <class T >
    int32 GetComponents( std::vector<int32>& identifiers, std::vector<T*>* components ) const;

    template <class T>
    int32 GetNextAvailableIndex() const;

    template <class T>
    bool ReleaseComponent( int32 identifier );

  private:
    ComponentPool* GetComponentPool( ComponentType type ) const;

};

template <class T >
T*
ComponentCache::GetComponent( int32 identifiers ) const
{
  ComponentPool* pool  = GetComponentPool( T::GetType() ); 
  if( pool != NULL )
  {
    return pool->GetComponent(identifier);
  }

  return NULL; 
}

template <class T >
int32
ComponentCache::GetComponents( std::vector<int32>& identifiers, std::vector<T*>* components ) const
{
  int32          count = 0;
  ComponentPool* pool  = GetComponentPool( T::GetType() );
  
  if( pool != NULL )
  {
    std::for_each( identifiers.begin(), identifiers.end(), [&](int32 identifier){ 
      T* component = static_cast<T*>(pool->GetComponent(identifier));
      if( component != NULL ) {
        count++;
        if( components != NULL ) {
          components->push_back( component );
        }
      }
    });
  }

  return count; 
}

template <class T >
int32 
ComponentCache::GetNextAvailableIndex() const
{
  ComponentPool* pool = GetComponentPool( T::GetType() );
  if( pool != NULL )
  {
    return pool->GetNextAvailableIndex();
  }

  return Component::UNUSED; 
}

}

#endif //__MODEL_CORE_COMPONENTCACHE_H__