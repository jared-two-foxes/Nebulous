#ifndef __COMPONENT_PLACEMENTCOMPONENT_H__
#define __COMPONENT_PLACEMENTCOMPONENT_H__

#include <Samples/Entity/Core/Component.h>

namespace Sample
{

class PlacementComponent : public Component
///
/// Component describes a world position and orientation that the object resides at.
///
{
public:
  
  PlacementComponent() 
    : position( 0,0,0,0 )
    , rotation( 0,0,0,1 )
    {}

  Vector4    position;
  Quaternion rotation;

  static ComponentType GetType() { return COMPONENT_TYPE_PLACEMENT; }

};

}

#endif __COMPONENT_PLACEMENTCOMPONENT_H__