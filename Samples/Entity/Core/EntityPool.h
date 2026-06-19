#ifndef NEBULAE_ENTITY_CORE_ENTITYPOOL_H_
#define NEBULAE_ENTITY_CORE_ENTITYPOOL_H_

#include <Nebulae/Common/Common.h>

namespace Sample
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
  std::deque<Entity*> m_availableEntities; ///< List of Entities that are not in use.
  std::vector<Entity*> m_usedEntities;     ///< List of Entities that are being used.

public:
  EntityPool();
  ~EntityPool();

  void Init( uint32 entityCount );

  void Clear();
  Entity* SpawnEntity( const char* uniqueName = 0 );
  Entity* FindEntityByIdentifier( const char* uniqueName ) const;

  const std::vector<Entity*>& GetEntities() const;
};

} // namespace Sample

#endif // NEBULAE_ENTITY_CORE_ENTITYPOOL_H_
