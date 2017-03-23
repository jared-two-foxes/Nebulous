#ifndef MODEL_COMPONENT_COLLISIONCOMPONENT_H__
#define MODEL_COMPONENT_COLLISIONCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Game {

class CollisionComponent : public Component
///
/// Component describes information for determining Collisions.
///
{
public:
  Vector4 center;
  Vector4 halfExtents;

  public:
    CollisionComponent() : Component(), 
      center(0,0,0,0),
      halfExtents(0,0,0,0)
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_COLLISION; }  
};

template <>
struct component_loader<CollisionComponent>
{
  static const char* component_name() { return "collision"; }

  static CollisionComponent& load( const Json::Value& value, CollisionComponent& component ) 
  { 
    component.halfExtents = Nebulae::json_cast_with_default<Vector4>( value["bounds"], Vector4(0,0,0,0) );

    return component; 
  }
};

}

#endif // MODEL_COMPONENT_COLLISIONCOMPONENT_H__