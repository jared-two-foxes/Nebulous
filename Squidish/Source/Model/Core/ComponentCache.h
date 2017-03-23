#ifndef __MODEL_CORE_COMPONENTCACHE_H__
#define __MODEL_CORE_COMPONENTCACHE_H__

#include <Nebulae/Common/Common.h>

#include <Model/Core/ComponentPool.h>
#include <Model/Core/ComponentType.h>

namespace Game {

class ComponentCache
///
/// Encapsulates the various different components available to Entities in the game system
/// and provides an interface for accessing each component and flagging a Component as being used. 
///
{
private:
  std::vector<ComponentPool* > m_pools; ///< List of the ComponentPools.  One for each ComponentType.
  
  public:
    ComponentCache();
    ~ComponentCache();

    void  Clear();
    void  AddComponentPool( ComponentPool* pool );
    void  RemoveComponentPool( ComponentPool* pool );
    int32 GetNextAvailableIndex( ComponentType type ) const;
    void  AllocateComponent( ComponentType type, int32 component, int32 allocatedTo );
    bool  ReleaseComponent( ComponentType type, int32 component, int32 deallocatedFrom );

    template <class T >
    T* GetComponent( int32 identifier ) const;

    template <class T>
    int32 GetComponents( std::vector<int32>& identifiers, std::vector<T*>* components ) const;

  private:
    ComponentPool* GetComponentPool( ComponentType type ) const;

};

template <class T >
T*
ComponentCache::GetComponent( int32 identifier ) const
{
  ComponentPool* pool  = GetComponentPool( T::GetType() ); 
  if( pool != NULL )
  {
    return static_cast<T*>( pool->GetComponent(identifier) );
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

}

#endif //__MODEL_CORE_COMPONENTCACHE_H__