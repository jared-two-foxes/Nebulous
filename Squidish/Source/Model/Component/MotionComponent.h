#ifndef MODEL_COMPONENT_MOTIONCOMPONENT_H__
#define MODEL_COMPONENT_MOTIONCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Game
{

class MotionComponent : public Component
///
/// Component describes the current motion of an Entity.
///
{
public:
  Vector4 velocity;
  Real    maximumVelocity;
  Vector4 acceleration;

  public:
    MotionComponent() : Component()
      , velocity(0,0,0,0)
      , maximumVelocity(2000.0f)
      , acceleration(0,0,0,0)
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_MOTION; }
};

template <>
struct component_loader<MotionComponent>
{
  static const char* component_name() { return "motion"; }

  static MotionComponent& load( const Json::Value& value, MotionComponent& component ) 
  { 
    component.velocity        = Nebulae::json_cast_with_default<Vector4>( value["velocity"],        Vector4(0,0,0,0) );
    component.maximumVelocity = Nebulae::json_cast_with_default<Real>(    value["maximumVelocity"], std::numeric_limits<Real>::max() );
    component.acceleration    = Nebulae::json_cast_with_default<Vector4>( value["acceleration"],    Vector4(0,0,0,0) );

    return component; 
  }
};

}

#endif // MODEL_COMPONENT_MOTIONCOMPONENT_H__