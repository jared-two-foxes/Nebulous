#ifndef NEBULAE_ENTITY_SYSTEMS_MOTIONSYSTEM_H_
#define NEBULAE_ENTITY_SYSTEMS_MOTIONSYSTEM_H_

#include <Nebulae/Common/Common.h>

#include "BoundingBox.h"

namespace Nebulae
{
class DebugUtil;
}

namespace Sample
{

class Entity;

class MotionSystem
{
private:
  BoundingBox m_boundingBox; ///< Constraining area for motion.

public:
  MotionSystem();

  void Process( Real elapsed, std::vector<Entity*>& entities );
  void DebugDraw( Nebulae::DebugUtil& renderer );
};

} // namespace Sample

#endif // NEBULAE_ENTITY_SYSTEMS_MOTIONSYSTEM_H_