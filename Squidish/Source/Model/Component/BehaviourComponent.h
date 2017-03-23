#ifndef __MODEL_COMPONENT_BEHAVIOURCOMPONENT_H__
#define __MODEL_COMPONENT_BEHAVIOURCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Nebulae {
  class SpriteAnimation;
}

namespace Game
{

class BehaviourComponent : public Component
///
/// Base behaviour component type.
///
{
public:
  float m_attractionStrength;
  float m_attractionInfluenceRadius;

  public:
    BehaviourComponent() 
    : Component(), 
      m_attractionStrength(0),
      m_attractionInfluenceRadius(0)      
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_BEHAVIOUR; }
    
};

template <>
struct component_loader<BehaviourComponent >
{
  static const char* component_name() { return "behaviour"; }

  static BehaviourComponent& load( const Json::Value& value, BehaviourComponent& component ) 
  { 
    component.m_attractionStrength        = Nebulae::json_cast_with_default<float>( value["attractionStrength"],        0.0f );
    component.m_attractionInfluenceRadius = Nebulae::json_cast_with_default<float>( value["attractionInfluenceRadius"], 0.0f );
    
    return component; 
  }
};

}

#endif // __MODEL_COMPONENT_BEHAVIOURCOMPONENT_H__