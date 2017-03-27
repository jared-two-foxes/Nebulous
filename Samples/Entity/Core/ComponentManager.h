#ifndef __CORE_COMPONENTMANAGER_H__
#define __CORE_COMPONENTMANAGER_H__

#include <Nebulae/Common/Common.h>

namespace Sample
{

class Component;

class ComponentManagerBase
{
public:
  virtual Component* GetComponentAt( int identifier ) const;

};

}

#endif //__CORE_COMPONENTMANAGER_H__