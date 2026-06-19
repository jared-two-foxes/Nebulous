#ifndef NEBULAE_ENTITY_CORE_COMPONENTMANAGER_H_
#define NEBULAE_ENTITY_CORE_COMPONENTMANAGER_H_

#include <Nebulae/Common/Common.h>

namespace Sample
{

class Component;

class ComponentManagerBase
{
public:
  virtual Component* GetComponentAt( int identifier ) const;
};

} // namespace Sample

#endif // NEBULAE_ENTITY_CORE_COMPONENTMANAGER_H_