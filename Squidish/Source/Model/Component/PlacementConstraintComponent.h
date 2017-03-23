#ifndef MODEL_COMPONENT_PLACEMENTCONSTRAINTCOMPONENT_H__
#define MODEL_COMPONENT_PLACEMENTCONSTRAINTCOMPONENT_H__

#include <Model/Core/Component.h>

namespace Game
{

class PlacementConstraintComponent : public Component
///
/// Component describes the maximum bounded coordinates of an entity.
///
{
public:
  Real leftBound;
  Real rightBound;
  Real topBound;
  Real bottomBound;
  Real frontBound;
  Real backBound;

  public:
    PlacementConstraintComponent() : Component(),
      leftBound(std::numeric_limits<Real>::lowest()),
      rightBound(std::numeric_limits<Real>::max()),
      topBound(std::numeric_limits<Real>::max()),
      bottomBound(std::numeric_limits<Real>::lowest()),
      frontBound(std::numeric_limits<Real>::max()),
      backBound(std::numeric_limits<Real>::lowest())
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_PLACEMENTCONSTRAINT; }

};

template <>
struct component_loader<PlacementConstraintComponent>
{
  static const char* component_name() { return "placementconstraint"; }

  static PlacementConstraintComponent& load( const Json::Value& value, PlacementConstraintComponent& component ) 
  { 
    component.leftBound   = Nebulae::json_cast_with_default<Real>( value["leftBound"],   std::numeric_limits<Real>::lowest() );
    component.rightBound  = Nebulae::json_cast_with_default<Real>( value["rightBound"],  std::numeric_limits<Real>::max() );
    component.topBound    = Nebulae::json_cast_with_default<Real>( value["topBound"],    std::numeric_limits<Real>::max() );
    component.bottomBound = Nebulae::json_cast_with_default<Real>( value["bottomBound"], std::numeric_limits<Real>::lowest() );
    component.backBound   = Nebulae::json_cast_with_default<Real>( value["backBound"],   std::numeric_limits<Real>::lowest() );
    component.frontBound  = Nebulae::json_cast_with_default<Real>( value["frontBound"],  std::numeric_limits<Real>::max() );

    return component; 
  }
};

}

#endif // MODEL_COMPONENT_MOTIONCOMPONENT_H__