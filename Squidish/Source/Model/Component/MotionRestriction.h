#ifndef __SQUID_MODEL_COMPONENT_MOTIONRESTRICTION_H__
#define __SQUID_MODEL_COMPONENT_MOTIONRESTRICTION_H__

#include <Model/Core/Component.h>

namespace Game
{

class MotionRestrictionComponent : public Component
///
/// When present in a component defines a per component scale
/// factor for any motion.  Can be used to block motion in 
/// specific directions (by specifying 0.0) or apply dampening
/// (specifying [0,1]) or exageration (specifying > 1).  
///
{
public:
  Vector4 m_scale_factor; 

public:
  MotionRestrictionComponent()
  : Component()
    m_scale_factor(1.0f,1.0f,1.0f,1.0f)
    {}

};

}

#endif // __SQUID_MODEL_COMPONENT_MOTIONRESTRICTION_H__