#ifndef __MODEL_CORE_COMPONENTPOOL_H__
#define __MODEL_CORE_COMPONENTPOOL_H__

#include <Nebulae/Common/Common.h>

#include <Samples/Entity/Core/ComponentType.h>

namespace Sample {

class Component;

class ComponentPool
{
  public:
    virtual ~ComponentPool() {}

    virtual ComponentType GetType() const = 0;

    virtual Component* GetComponent( int32 identifier ) const = 0;
    
    virtual const int32 GetNextAvailableIndex() const = 0;

};

}

#endif // __MODEL_CORE_COMPONENTPOOL_H__ 