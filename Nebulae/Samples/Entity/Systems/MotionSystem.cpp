
#include "MotionSystem.h"
#include "Samples/Entity/Entities/Boid.h"
#include <Nebulae/Beta/Debug/DebugUtil.h>

namespace Sample
{
  
MotionSystem::MotionSystem()
///
/// Default Constructor.
///
  : m_boundingBox( Vector4(200.0f,200.0f,200.0f))
{

}

void 
MotionSystem::Process( Real elapsed, std::vector<Entity*>& entities )
///
/// Advance the motion state of the entities recieved.
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
    Boid&               boid      = *(static_cast<Boid*>(*it));
    MotionComponent&    motion    = boid.GetMotion();
    PlacementComponent& placement = boid.GetPlacement();

    placement.position   += motion.velocity * elapsed; 

    //@todo [jared.watt 285-08-2013] Constrain position to within the bounding box.
    Vector4 halfExtents = m_boundingBox.GetHalfExtents();
    placement.position.x = std::max<Real>(placement.position.x, -halfExtents.x );
    placement.position.x = std::min<Real>(placement.position.x,  halfExtents.x );
    placement.position.y = std::max<Real>(placement.position.y, -halfExtents.y );
    placement.position.y = std::min<Real>(placement.position.y,  halfExtents.y );
  }
}


void
MotionSystem::DebugDraw( Nebulae::DebugUtil& renderer )
///
/// Draws the outline of the containment area
///
/// @param renderer
///   The object to use to draw the debug lines.
///
/// @return
///   Nothing
///
{
  Vector4 min = -m_boundingBox.GetHalfExtents();
  Vector4 max = m_boundingBox.GetHalfExtents();
  Vector4 colour(1.0f,0.0f,0.0f,1.0f);
  Vector4 v[8];

  v[0].set(min.x, min.y, min.z);
  v[1].set(max.x, min.y, min.z);
  v[2].set(max.x, min.y, max.z);
  v[3].set(min.x, min.y, max.z);

  v[4].set(min.x, max.y, min.z);
  v[5].set(max.x, max.y, min.z);
  v[6].set(max.x, max.y, max.z);
  v[7].set(min.x, max.y, max.z);

  renderer.AddLine(v[0], v[1], colour);
  renderer.AddLine(v[1], v[2], colour);
  renderer.AddLine(v[2], v[3], colour);
  renderer.AddLine(v[3], v[0], colour);

  renderer.AddLine(v[4], v[5], colour);
  renderer.AddLine(v[5], v[6], colour);
  renderer.AddLine(v[6], v[7], colour);
  renderer.AddLine(v[7], v[4], colour);

  renderer.AddLine(v[0], v[4], colour);
  renderer.AddLine(v[1], v[5], colour);
  renderer.AddLine(v[2], v[6], colour);
  renderer.AddLine(v[3], v[7], colour);
}

}