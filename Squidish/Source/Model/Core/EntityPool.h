#ifndef __CORE_ENTITYMANAGER_H__
#define __CORE_ENTITYMANAGER_H__

#include <Nebulae/Common/Common.h>

namespace Game
{
  
class Entity;

class EntityPool
///
/// A container for a non-resizeable set of Entity objects.  The EntityPool maintains control of 
/// the lifetime of the objects contained including the creation and destruction of Entity objects
/// and manages how many Entity objects are created at a time.
///
{
private:
  std::deque<Entity* >  m_availableEntities; ///< List of Entities that are not in use.
  std::vector<Entity* > m_usedEntities;      ///< List of Entities that are being used. 

  public:
    EntityPool();
    ~EntityPool();

    void Init( uint32 entityCount );

    void    Clear();
    Entity* SpawnEntity( const char* uniqueName = 0 );
    Entity* FindEntityByIdentifier( int32 identifier ) const;
    Entity* FindEntityByUniqueName( const char* uniqueName ) const;

    const std::vector<Entity*>& GetEntities() const;
  
};

}

#endif // __CORE_ENTITYMANAGER_H__
