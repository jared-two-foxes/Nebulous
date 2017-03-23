#ifndef MODEL_COMPONENT_ANIMATIONCOMPONENT_H__
#define MODEL_COMPONENT_ANIMATIONCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Nebulae {
  class SpriteAnimation;
}

namespace Game
{

class AnimationComponent : public Component
///
/// Component describes a single animation sequence.
///
{
public:
  std::string               animationName;  ///< Name of the Animation resource to use.
  Nebulae::SpriteAnimation* animation;      ///< SpriteAnimation to take the frame from.
  int                       currentFrame;   ///< The current frame that this instance is currently on
  float                     currentElapsed; ///< time which the current frame has been visible.

  public:
    AnimationComponent() 
    : Component(), 
      animationName(),
      animation(NULL),
      currentFrame(-1),
      currentElapsed(0.f)
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_ANIMATION; }
    
};

template <>
struct component_loader<AnimationComponent>
{
  static const char* component_name() { return "animation"; }

  static AnimationComponent& load( const Json::Value& value, AnimationComponent& component ) 
  { 
    component.currentFrame   = Nebulae::json_cast_with_default<int>(         value["currentFrame"],    0 );
    component.currentElapsed = Nebulae::json_cast_with_default<float>(       value["currentElapsed"],  0.f );
    component.animationName  = Nebulae::json_cast_with_default<const char*>( value["animation"],       "" );

    return component; 
  }
};

}

#endif // MODEL_COMPONENT_ANIMATIONCOMPONENT_H__