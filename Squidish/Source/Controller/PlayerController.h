#ifndef CONTROLLER_CONTROLLERS_PLAYERCONTROLLER_H__
#define CONTROLLER_CONTROLLERS_PLAYERCONTROLLER_H__

#include <Nebulae/Nebulae.h>
#include <Nebulae/Common/Window/KeyCodes.h>

namespace Nebulae {
  class Camera;
}

namespace Game {

//Forward Deceleration
class Entity;
class Model;
class MotionSystem;
class PlacementConstraintComponent;

class PlayerController
///
/// Controls the behaviour of the player character and defines all possible responses to user input.
///
{
public:
  typedef std::shared_ptr<Model > ModelPtr;

public:
  ModelPtr                m_model;
  Entity*                 m_player;
  MotionSystem*           m_motionSystem;
  Nebulae::Camera&        m_camera;
  Real                    m_pulseTimeRemaining; ///< Amount of time remaining until the next pulse is allowed.  (0.05f)


  //
  // Replace the following with a single int with bit flags.
  // 
  bool m_moveLeft;
  bool m_moveRight;


  public:
    /** \name Structors */ ///@{
    PlayerController( const ModelPtr& model, Nebulae::Camera& camera );
    //@}

    /** \name Mutators */ ///@{
    void Clear();
    void Init( MotionSystem* motionSystem, Entity* entity );
    void OnKeyPressed( Nebulae::KeyCode keyCode );
    void OnKeyReleased( Nebulae::KeyCode keyCode );
    void OnTouchDown( int x, int y );
    void OnTouchUp( int x, int y );
    void OnTouchDrag( int x, int y );
    void Update( const float elapsed );

    void UpdatePlacementConstraint( PlacementConstraintComponent* placementConstraintComponent, uint32 width, uint32 height );
    //@}

};

}

#endif // CONTROLLER_CONTROLLERS_PLAYERCONTROLLER_H__