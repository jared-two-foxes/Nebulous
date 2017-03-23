#ifndef __SQUIDISH_INTELLIGENCESYTEM_H__
#define __SQUIDISH_INTELLIGENCESYTEM_H__

#include <Nebulae/Common/Common.h>

namespace Game {

class Entity;
class Model;
class MotionSystem;

class IntelligenceSystem
///
/// Each system/controller defines a discrete subsystem used to add a
/// specific type of functionality to the game.
///
/// The Intelligence subsystem is responsible for determining how the
/// Entities should behave in any situation based upon data in the 
/// IntelligenceComponent.  In Squidish the bahvaiour of Entities is
/// based upon a system of "Steering Behaviours" thus this system
/// has a dependency on the MotionSystem.
///
{
public:
  typedef std::shared_ptr<Model >        ModelPtr;

private:
  ModelPtr      m_model;
  MotionSystem* m_motionSystem;

  public:
    IntelligenceSystem( ModelPtr model, MotionSystem* motionSystem );

    void Process( const std::vector<Entity*>& entities, Entity* playerEntity );

};

}

#endif // __SQUIDISH_INTELLIGENCESYTEM_H__