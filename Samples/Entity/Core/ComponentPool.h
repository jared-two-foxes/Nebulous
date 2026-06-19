#ifndef NEBULAE_ENTITY_CORE_COMPONENTPOOL_H_
#define NEBULAE_ENTITY_CORE_COMPONENTPOOL_H_

#include <Nebulae/Common/Common.h>

#include <Samples/Entity/Core/ComponentType.h>

namespace Sample
{

class Component;

class ComponentPool
{
public:
  virtual ~ComponentPool() {}

  virtual ComponentType GetType() const = 0;

  virtual Component* GetComponent( int32 identifier ) const = 0;

  virtual const int32 GetNextAvailableIndex() const = 0;
};

} // namespace Sample

#endif // NEBULAE_ENTITY_CORE_COMPONENTPOOL_H_