#ifndef __COMPONENT_MOTIONCOMPONENT_H__
#define __COMPONENT_MOTIONCOMPONENT_H__

#include <Samples/Entity/Core/Component.h>

namespace Sample
{

class MotionComponent : public Component
///
/// Component describes
///
{
public:

  MotionComponent()
    : velocity(0,0,0,0)
    , maximumVelocity(10.0f)
    , acceleration(0,0,0,0)
    {}

  Vector4 velocity;
  Real    maximumVelocity;
  Vector4 acceleration;

  static ComponentType GetType() { return COMPONENT_TYPE_MOTION; }

};

}

#endif __COMPONENT_MOTIONCOMPONENT_H__