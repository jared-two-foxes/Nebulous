#ifndef MODEL_COMPONENT_SCORECOMPONENT_H__
#define MODEL_COMPONENT_SCORECOMPONENT_H__

#include <Model/Core/Component.h>

namespace Game {

class ScoreComponent : public Component
///
/// Component contains information related to the score bonus the user gets for.
/// collecting this entity.
///
{
public:
  int32 bonus;

  public:
    ScoreComponent() : Component(), 
      bonus(0)
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_SCORE; }

};


template <>
struct component_loader<ScoreComponent>
{
  static const char* component_name() { return "score"; }

  static ScoreComponent& load( const Json::Value& value, ScoreComponent& component ) 
  { 
    component.bonus = Nebulae::json_cast_with_default<int32>( value["bonus"], 0 );
    
    return component; 
  }
};
}

#endif // MODEL_COMPONENT_SCORECOMPONENT_H__