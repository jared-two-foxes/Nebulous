
#include "MotionSystem.h"

#include <Model/Model.h>
#include <Model/Component/MotionComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Component/PlacementConstraintComponent.h>
#include <Model/Core/Entity.h>

using namespace Game;

  
MotionSystem::MotionSystem( ModelPtr model )
///
/// Default Constructor
///
: m_active( true ),
  m_model( model )
{}


bool
MotionSystem::IsActive() const
///
/// Returns if the Motion system is currently allowed to process entities.
///
/// @return 
///   true  - processing entities.
///   false - paused.
///
{ return m_active; }


void 
MotionSystem::ApplyAttraction( Entity* obj1, Entity* obj2, Real strength )
{
  Vector4 target     = GetPosition( m_model, *obj2 );
  Vector4 position   = GetPosition( m_model, *obj1 );
  Vector4 direction  = target - position; 
  direction.normalize();
  direction * strength;
  ApplySteeringForce( *obj1, direction );
}

void 
MotionSystem::ApplySeperation( Entity* obj1, Entity* obj2, Real strength )
{
  Vector4 target     = GetPosition( m_model, *obj2 );
  Vector4 position   = GetPosition( m_model, *obj1 );
  Vector4 direction  = position - target; 
  direction.normalize();
  direction * strength;
  ApplySteeringForce( *obj1, direction );
}

void
MotionSystem::ApplySteeringForce( const Entity& entity, const Vector4& steeringForce )
{
  std::vector<EntitySteeringForcePair >::iterator it, end_it = m_steeringForces.end();
  for( it = m_steeringForces.begin(); it != end_it; ++it ) {
    if( (*it).first == &entity ) {
      (*it).second += steeringForce;
      break;
    }
  }

  if( it == end_it ) {
    Vector4 parameter( steeringForce );
    m_steeringForces.push_back( std::pair<Entity*, Vector4>( const_cast<Entity*>( &entity ), parameter ) );
  }
}


void
MotionSystem::ApplyAcceleration( const Entity& entity, const Vector4& acceleration )
///
/// Applies an acceleration value to an Entity 
///
/// @param entity
///   The entity to apply acceleration to.
///
/// @param acceleration
///   The acceleration to apply.
///
/// @return
///   Nothing.
///
{
  std::vector<MotionComponent* > motionComponents; 
  m_model->GetComponents<MotionComponent>( &entity, &motionComponents );
  
  std::for_each( motionComponents.begin(), motionComponents.end(), [&](MotionComponent* c) {
    c->acceleration += acceleration;
  });
}


void 
MotionSystem::SetActive( bool isActive )
///
/// sets the active flag.
/// 
/// @param isActive
///   The value to set.
/// 
/// @return
///   Nothing.
/// 
{
  m_active = isActive;
}


void 
MotionSystem::SetAcceleration( const Entity& entity, const Vector4& acceleration )
///
/// Sets the acceleration of a given entity.
///
/// @param entity.
///   The entity to apply acceleration to.
///
/// @param acceleration
///   The acceleration to apply.
///
/// @return
///   Nothing.
///
{
  std::vector<MotionComponent* > motionComponents; 
  m_model->GetComponents<MotionComponent>( &entity, &motionComponents );
  
  std::for_each( motionComponents.begin(), motionComponents.end(), [&](MotionComponent* c) {
    c->acceleration = acceleration;
  });
}


void 
MotionSystem::SetVelocity( const Entity& entity, const Vector4& velocity )
///
/// Sets the acceleration of a given entity.
///
/// @param entity.
///   The entity to apply acceleration to.
///
/// @param acceleration
///   The acceleration to apply.
///
/// @return
///   Nothing.
///
{
  std::vector<MotionComponent* > motionComponents; 
  m_model->GetComponents<MotionComponent>( &entity, &motionComponents );
  
  std::for_each( motionComponents.begin(), motionComponents.end(), [&](MotionComponent* c) {
    c->velocity = velocity;
  });
}


void
MotionSystem::Process( const std::vector<Entity*>& entities, float elapsed )
///
/// Updates the position of all entities that have a motion component.
///
/// @return
///   Nothing.
///
{
  if( ! m_active ) return;
  
  //process!
  std::vector<Entity*>::const_iterator end_entity_it = entities.end();
  for( std::vector<Entity*>::const_iterator entity_it = entities.begin(); entity_it != end_entity_it; ++entity_it ) {
    
    if( !(*entity_it)->IsEnabled() ) continue;

    std::vector<MotionComponent* >              motionComponents;
    std::vector<PlacementComponent* >           placementComponents; 
    std::vector<PlacementConstraintComponent* > placementConstraintComponents;

    m_model->GetComponents<MotionComponent>( (*entity_it), &motionComponents );
    m_model->GetComponents<PlacementComponent>( (*entity_it), &placementComponents );
    m_model->GetComponents<PlacementConstraintComponent>( (*entity_it), &placementConstraintComponents );
            
    // determine if this entity has the appropriate "aspect" component for this system to draw.
    if( !motionComponents.empty() && !placementComponents.empty() ) {
      
      // check & apply the entities steering force.
      Vector4 steeringForce(0,0,0,0);
      std::vector<EntitySteeringForcePair >::iterator it, end_it = m_steeringForces.end();
      for( it = m_steeringForces.begin(); it != end_it; ++it ) {
        if( (*it).first == (*entity_it) ) {
          steeringForce += (*it).second;
          (*it).second = Vector4(0,0,0,0);
          break;
        }
      }

      motionComponents[0]->velocity += steeringForce;

      if( fabs(motionComponents[0]->velocity.length()) > motionComponents[0]->maximumVelocity ) { 
        // truncate velocity to the maximum length.
        float i = motionComponents[0]->maximumVelocity / motionComponents[0]->velocity.length();
        i = (i > 1.0f) ? 1.0f : i; //to ensure that we don't make the vector longer than it currently is.
        motionComponents[0]->velocity *= i;
      }

      placementComponents[0]->position += motionComponents[0]->velocity * elapsed;     

      if( !placementConstraintComponents.empty() ) {
        placementComponents[0]->position.x = std::max<Real>( placementConstraintComponents[0]->leftBound, placementComponents[0]->position.x );
        placementComponents[0]->position.x = std::min<Real>( placementConstraintComponents[0]->rightBound, placementComponents[0]->position.x );

        placementComponents[0]->position.y = std::min<Real>( placementConstraintComponents[0]->bottomBound, placementComponents[0]->position.y );
        placementComponents[0]->position.y = std::max<Real>( placementConstraintComponents[0]->topBound, placementComponents[0]->position.y );
        
        placementComponents[0]->position.z = std::max<Real>( placementConstraintComponents[0]->backBound, placementComponents[0]->position.z );
        placementComponents[0]->position.z = std::min<Real>( placementConstraintComponents[0]->frontBound, placementComponents[0]->position.z );
      }
    }
  }
}




Vector4 
MotionSystem::GetPosition( ModelPtr model, const Entity& entity )
///
/// Returns the current Position of an Entity.
///
/// @param entity
///   The entity to retrieve the position of.
///
/// @return
///   If the Entity has a PlacementComponent then the function return's the Entity's position, else
///   it will return a "zeroed" vector.
///
{
  std::vector<PlacementComponent* > placementComponents; 
  model->GetComponents<PlacementComponent>( &entity, &placementComponents );

  if( !placementComponents.empty() ) {
    return placementComponents[0]->position;
  }

  return Vector4(0,0,0,0);
}


Vector4 
MotionSystem::GetVelocity( ModelPtr model, const Entity& entity )
///
/// Returns the current Velocity of an Entity.
///
/// @param entity
///   The entity to retrieve the velocity of.
///
/// @return
///   If the Entity has a MotionComponent then the function return's the Entity's velocity, else
///   it will return a "zeroed" vector.
///
{
  std::vector<MotionComponent* > motionComponents; 
  model->GetComponents<MotionComponent>( &entity, &motionComponents );

  if( !motionComponents.empty() ) {
    return motionComponents[0]->velocity;
  }

  return Vector4(0,0,0,0);
}


Real
MotionSystem::GetMaximumVelocity( ModelPtr model, const Entity& entity )
{
  std::vector<MotionComponent* > motionComponents; 
  model->GetComponents<MotionComponent>( &entity, &motionComponents );

  if( !motionComponents.empty() ) {
    return motionComponents[0]->maximumVelocity;
  }

  return 0;
}


Vector4 
MotionSystem::GetAcceleration( ModelPtr model, const Entity& entity )
///
/// Returns the current Acceleration of an Entity.
///
/// @param entity
///   The entity to retrieve the acceleration for.
///
/// @return
///   If the Entity has a MotionComponent then the function return's the Entity's acceleration, else
///   it will return a "zeroed" vector.
///
{
  std::vector<MotionComponent* > motionComponents; 
  model->GetComponents<MotionComponent>( &entity, &motionComponents );

  if( !motionComponents.empty() ) {
    return motionComponents[0]->acceleration;
  }

  return Vector4(0,0,0,0);
}

Vector4 
MotionSystem::GetDistanceBetweenEntities( ModelPtr model, const Entity& entity, const Entity& target )
///
/// Calculates the vector between the entity and target variables.  Assumes that both entities
/// have a PlacementComponent.
///
/// @param entity, target
///   Entity objects to determine the distance between.
///
/// @return
///   The Vector from the 'entity' to the 'target'
///
{
  std::vector<PlacementComponent* > entityComponents, targetComponents; 

  model->GetComponents<PlacementComponent >( &entity, &entityComponents );
  model->GetComponents<PlacementComponent >( &target, &targetComponents );

  NE_ASSERT( entityComponents.size() == 1, "Entity does not have an associated PlacementComponent" )( /*entity*/ );
  NE_ASSERT( targetComponents.size() == 1, "Entity does not have an associated PlacementComponent" )( /*target*/ );

  return targetComponents[0]->position - entityComponents[0]->position;
}