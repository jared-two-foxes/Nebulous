
#include "IntelligenceSystem.h"
#include "Samples/Entity/Entities/Boid.h"

namespace Sample
{
  
IntelligenceSystem::IntelligenceSystem()
///
/// Default Constructor.
///
{

}


void 
IntelligenceSystem::Process( Real elapsed, std::vector<Entity*>& entities )
///
/// Determines how the entities should behave at this point of the simulation.
///
/// @param elapsed
///   Time to advance the simulation
/// 
/// @param entities
///   list of the live entities to simulate
///
/// @return
///   Nothing
///
{
  std::vector<Entity*>::iterator end_it = entities.end();
  for( std::vector<Entity*>::iterator it = entities.begin(); it != end_it; ++it )
  {
    Boid&               boid             = *static_cast<Boid*>(*it);
    PlacementComponent& placement        = boid.GetPlacement();
    MotionComponent&    motion           = boid.GetMotion();
    float               mass             = 1.0f;
    const float         seperationWeight = 12.5f;
    const float         cohesionWeight   = 1.0f;
    const float         alignmentWeight  = 1.0f;

    Vector4             steeringForce    = Vector4(0,0,0,0);

    //determine the steering behaviour for current entity.
    steeringForce         += Separation( **it, entities ) * seperationWeight;
    steeringForce         += Cohesion( **it, entities ) * cohesionWeight;
    steeringForce         += Alignment( **it, entities ) * alignmentWeight;

    if( steeringForce.length() > 2.0f )
    {
      steeringForce.normalize();
      steeringForce *= 2.0f;
    }

    Vector4 acceleration  = steeringForce / mass;

    motion.velocity       += acceleration * elapsed;

    if( motion.velocity.length() > motion.maximumVelocity )
    {
      motion.velocity.normalize();
      motion.velocity *= motion.maximumVelocity;
    }
  }
}


Vector4 
IntelligenceSystem::Seek( Entity& entity, const Vector4& position ) const
///
/// Determines what force to apply to an entity to get it to navigate towards a given point.
///
/// @param entity
///   The entity to navigate with.
///
/// @param position
///   The position to navigate to.
///
/// @return
///   The force to apply to the entity.
///
{
  Boid&               boid            = *static_cast<Boid*>(&entity);
  PlacementComponent& placement       = boid.GetPlacement();
  MotionComponent&    motion          = boid.GetMotion();
  Vector4             desiredVelocity = (position - placement.position).normalize() * motion.maximumVelocity;

  return (desiredVelocity - motion.velocity);
}


Vector4 
IntelligenceSystem::Flee( Entity& entity, const Vector4& position ) const
///
/// Determines what force to apply to an entity to get it to navigate awawy from a given point.
///
/// @param entity
///   The entity to navigate with.
///
/// @param position
///   The position to navigate away from.
///
/// @return
///   The force to apply to the entity.
///
{
  Boid&               boid            = *static_cast<Boid*>(&entity);
  PlacementComponent& placement       = boid.GetPlacement();
  MotionComponent&    motion          = boid.GetMotion();
  Vector4             desiredVelocity = (placement.position - position).normalize() * motion.maximumVelocity;

  return (desiredVelocity - motion.velocity);
}


Vector4 
IntelligenceSystem::Alignment( Entity& entity, const std::vector<Entity*>& neighbors ) const
/// @todo [jared.watt 14-08-2013] This function needs documentation.
{
  Boid&               boid            = *static_cast<Boid*>(&entity);
  PlacementComponent& placement       = boid.GetPlacement();
  MotionComponent&    motion          = boid.GetMotion();
  Vector4             steeringForce   = Vector4(0,0,0,0);
  Quaternion          averageHeading  = Quaternion(0,0,0,1);
  int                 neighborCount   = 0;

  //iterate through all the tagged vehicles and sum their heading vectors  
  for (unsigned int a=0; a<neighbors.size(); ++a)
  {
    //make sure *this* agent isn't included in the calculations and that
    //the agent being examined  is close enough ***also make sure it doesn't
    //include any evade target ***
    if( neighbors[a] != &entity )
    {
      averageHeading += static_cast<Boid*>(neighbors[a])->GetPlacement().rotation;
      ++neighborCount;
    }
  }

  //if the neighborhood contained one or more vehicles, average their
  //heading vectors.
  if( neighborCount > 0 )
  {
    averageHeading /= (double)neighborCount;
    averageHeading -= placement.rotation;
  }
  
  return averageHeading.m_vec;
}


Vector4
IntelligenceSystem::Cohesion( Entity& entity, const std::vector<Entity*>& neighbors ) const
/// @todo [jared.watt 14-08-2013] This function needs documentation.
{
  Boid&               boid            = *static_cast<Boid*>(&entity);
  PlacementComponent& placement       = boid.GetPlacement();
  MotionComponent&    motion          = boid.GetMotion();
  Vector4             steeringForce   = Vector4(0,0,0,0);
  Vector4             centerOfMass    = Vector4(0,0,0,0);
  int                 neighborCount   = 0;

  for( uint32 a = 0; a < neighbors.size(); ++a )
  {
    if( neighbors[a] != &entity )
    {
      centerOfMass += static_cast<Boid*>(neighbors[a])->GetPlacement().position;
      centerOfMass.w = 0;

      ++neighborCount;
    }
  }

  if( neighborCount > 0 )
  {
    centerOfMass  /= (double)neighborCount;
    steeringForce =  Seek(entity, centerOfMass);
  }

  //the magnitude of cohesion is usually much larger than separation or
  //allignment so it usually helps to normalize it.
  return steeringForce.normalize();
}


Vector4 
IntelligenceSystem::Separation( Entity& entity, const std::vector<Entity*> &neighbors ) const
/// @todo [jared.watt 14-08-2013] This function needs documentation.
{  
  Boid&               boid            = *static_cast<Boid*>(&entity);
  PlacementComponent& placement       = boid.GetPlacement();
  MotionComponent&    motion          = boid.GetMotion();
  Vector4             steeringForce   = Vector4(0,0,0,0);

  for( uint32 a = 0; a < neighbors.size(); ++a )
  {
    if( neighbors[a] != &entity )
    {
      Vector4 displacement = placement.position - static_cast<Boid*>(neighbors[a])->GetPlacement().position;
      displacement.w = 0;

      //scale the force inversely proportional to the agents distance from its neighbor.
      Real length    = displacement.length();
      steeringForce += displacement.normalize()/length;
    }
  }

  return steeringForce;
}

}