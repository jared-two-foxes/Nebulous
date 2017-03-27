#ifndef __ENTITIES_BOID_H__
#define __ENTITIES_BOID_H__


#include <Samples/Entity/Core/Entity.h>
#include <Samples/Entity/Core/EntityManager.h>
#include <Samples/Entity/Components/MotionComponent.h>
#include <Samples/Entity/Components/PlacementComponent.h>

namespace Sample
{

#define BOID 1
  
class Boid : public Entity
{
private:
  PlacementComponent m_placement;
  MotionComponent    m_motion;

public:
  Boid( uint32 identifier ) : Entity(identifier) {}
  virtual ~Boid() {}

  virtual int       GetType() const        { return BOID; }

  PlacementComponent&       GetPlacement()       { return m_placement; }
  const PlacementComponent& GetPlacement() const { return m_placement; }
  MotionComponent&          GetMotion()          { return m_motion; }
  const MotionComponent&    GetMotion() const    { return m_motion; }

};

class BoidAllocator : public EntityManager::Allocator
{
  virtual Boid* Create() const {
    static int s_nextIdentifier = 0;
    return new Boid( s_nextIdentifier++ );
  }
};

}

#endif // __ENTITIES_BOID_H__