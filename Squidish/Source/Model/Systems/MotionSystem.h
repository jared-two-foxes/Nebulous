#ifndef CONTROLLER_SYSTEMS_MOTIONSYSTEM_H__
#define CONTROLLER_SYSTEMS_MOTIONSYSTEM_H__

#include <Nebulae/Nebulae.h>

namespace Game
{

//Forward Deceleration
class Entity;
class Model;

class MotionSystem
{
public:
  typedef std::pair<Entity*, Vector4> EntitySteeringForcePair;

  typedef std::shared_ptr<Model > ModelPtr;

public:    
  bool                                  m_active;
  ModelPtr                              m_model;
  std::vector<EntitySteeringForcePair > m_steeringForces;

  public:
    /** \name Structors */ ///@{
    MotionSystem( ModelPtr model );
    //@}

    /** \name Accessors */ ///@{
    bool    IsActive() const;
    //@} 


    /** \name Mutators */ ///@{
    void SetActive( bool isActive );
    void SetAcceleration( const Entity& entity, const Vector4& acceleration );
    void SetVelocity( const Entity& entity, const Vector4& velocity );
      
    void ApplyAttraction( Entity* obj1, Entity* obj2, Real strength );
    void ApplySeperation( Entity* obj1, Entity* obj2, Real strength );

    void ApplySteeringForce( const Entity& entity, const Vector4& steeringForce );
    void ApplyAcceleration( const Entity& entity, const Vector4& acceleration );
    void Process( const std::vector<Entity*>& entities, float elapsed );
    //@}

    

    // Investigate passing the model as a paramter rather than making these member functions?
    static Vector4 GetPosition( ModelPtr model, const Entity& entity );
    static Vector4 GetVelocity( ModelPtr model, const Entity& entity );
    static Real    GetMaximumVelocity( ModelPtr model, const Entity& entity );
    static Vector4 GetAcceleration( ModelPtr model, const Entity& entity );

    static Vector4 GetDistanceBetweenEntities( ModelPtr model, const Entity& entity, const Entity& target );

};

}

#endif // CONTROLLER_SYSTEMS_MOTIONSYSTEM_H__