#ifndef __MODEL_CORE_COMPONENTPOOL_H__
#define __MODEL_CORE_COMPONENTPOOL_H__

#include <Nebulae/Common/Common.h>

#include <Model/Core/ComponentType.h>

namespace Game {

class Component;

class ComponentPool
{
  public:
    virtual ~ComponentPool() {}

    virtual void Clear() {}

    virtual ComponentType GetType() const = 0;

    virtual Component* GetComponent( int32 identifier ) = 0;
    
    virtual const int32 GetNextAvailableIndex() const = 0;
    virtual void AllocateComponent( int32 component, int32 allocatedTo ) = 0;
    virtual bool ReleaseComponent( int32 component, int32 deallocatedFrom ) = 0;

};

}

#endif // __MODEL_CORE_COMPONENTPOOL_H__ 