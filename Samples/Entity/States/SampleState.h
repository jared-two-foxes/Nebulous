#ifndef __STATES_SAMPLESTATE_H__
#define __STATES_SAMPLESTATE_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/State.h>

namespace Nebulae {
  class DebugUtil;
}

namespace Sample 
{

class EntityManager;
class IntelligenceSystem;
class MotionSystem;

class EntityRenderSystem;

class SampleState : public Nebulae::State
///
/// A simple state to demonstrate the usage of a basic Entity-Component type system.
///
{
private:
  //Model Systems
  EntityManager*      m_entityManager;      ///< The entities that make up the game world.  This could become _THE_ model  
  IntelligenceSystem* m_intelligenceSystem; ///< Controls the behaviour of each individual boid.
  MotionSystem*       m_motionSystem;       ///< Controls the simulating the chance of placement for each individual boid.

  // View Systems
  EntityRenderSystem* m_entityRenderer;     ///< The view object.  Controls how the game world is presented to the user. (View)
#ifdef _DEBUG
  Nebulae::DebugUtil* m_debugRenderer;
#endif

public:
  SampleState();
  virtual ~SampleState();
  
  virtual void Enter( Nebulae::StateStack* caller );
  virtual void Exit( Nebulae::StateStack* caller );
  virtual void Update( Real elapsed, Nebulae::StateStack* pCaller );
  virtual void Render() const;

}; //SampleState

}

#endif // __STATES_SAMPLESTATE_H__
