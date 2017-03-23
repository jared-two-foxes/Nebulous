#ifndef __COMPONENT_PLACEMENTCOMPONENT_H__
#define __COMPONENT_PLACEMENTCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Game
{

class PlacementComponent : public Component
///
/// Component describes a world position and orientation that the object resides at.
///
{
public:
  Vector4    position;
  Quaternion rotation;

  public:
    PlacementComponent() : Component()
      , position( 0,0,0,0 )
      , rotation( 0,0,0,1 )
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_PLACEMENT; }

};

template <>
struct component_loader<PlacementComponent>
{
  static const char* component_name() { return "placement"; }

  static PlacementComponent& load( const Json::Value& value, PlacementComponent& component ) 
  { 
    component.position = Nebulae::json_cast_with_default<Vector4>(    value["position"], Vector4(0,0,0,0) );
    component.rotation = Nebulae::json_cast_with_default<Quaternion>( value["rotation"], Quaternion(0,0,0,1) );

    return component; 
  }
};

}

#endif __COMPONENT_PLACEMENTCOMPONENT_H__