#ifndef CONTROLLER_STATES_GAMESCREEN_H__ 
#define CONTROLLER_STATES_GAMESCREEN_H__

#include <Nebulae/Common/Common.h>

#include <States/BaseScreen.h>

#include <Controller/CameraController.h>
#include <Controller/IntelligenceSystem.h>
#include <Controller/PlayerController.h>
#include <Controller/TrackGenerator.h>

#include <Model/Systems/AnimationSystem.h>
#include <Model/Systems/CollisionSystem.h>
#include <Model/Systems/MotionSystem.h>

namespace Nebulae 
{
  class TextControl;
}

namespace Game {

class GameScreen : public BaseScreen
///
/// This class encapsulates the main game state including the initial title screen.
///
/// It contains controller objects for each of the subsystems which are required
/// to simulate the rules as described by the design.
///
{
private:
  typedef std::unique_ptr<MotionSystem >          MotionSystemPtr;
  typedef std::unique_ptr<CollisionSystem >       CollisionSystemPtr;
  typedef std::unique_ptr<AnimationSystem >       AnimationSystemPtr;
  typedef std::unique_ptr<IntelligenceSystem >    IntelligenceSystemPtr;

public:
  enum State 
  {
    STATE_UNKNOWN,
    STATE_MENU,
    STATE_INTRO,
    STATE_ACTIVE,
    STATE_DEAD
  };

public:
  class ViewController;

private:
  State                 m_state;

  ViewController*       m_viewController;

  MotionSystem          m_motionSystem;
  CollisionSystem       m_collisionSystem;
  AnimationSystem       m_animationSystem;
  IntelligenceSystem    m_intelligenceSystem;
  TrackGenerator        m_generator;
                        
  CameraController      m_cameraController;
  PlayerController      m_playerController;

  int32                 m_currentScore;      ///< The score obtained by picking up powerups etc.
  Real                  m_godTime;           ///< Amount of time the user has remaining of god mode.
  Real                  m_introTimer;        ///< Amount of time remaining in the intro sequence.

  public:
    /** \name Structors */ ///@{
    GameScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
      const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
      const ModelPointer& model, const ViewPointer& view, ViewController* viewController );
    
    virtual ~GameScreen();
    //@}
    
    /** \name Accessors */ ///@{
    State GetState() const;
    const MotionSystem& GetMotionSystem() const;
    Entity* GetPlayerEntity() const;
    //@}


    /** \name Mutators */ ///@{
    virtual void SetState( State state );
    virtual void PlayAnimation( Entity& entity, const std::string& animationName );
    virtual void Restart();
    virtual void Continue();
    //@}

  private:
    /** \name Mutators */ ///@{
    void SetViewController( ViewController* viewController );
    
    virtual bool InvokeAction( const std::string& context );

    virtual void OnInit();
    virtual void OnEnter();
    virtual void OnExit();
    virtual bool OnKeyPressed(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnKeyReleased(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchDown(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchUp(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchDrag(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual void OnUpdate( float elapsed );
    void OnCollision( const Entity& obj1, const Entity& obj2 );
    void OnStateChanged( State state );
    virtual void OnGodModeBegun();
    virtual void OnGodModeEnded();
    //@}

    bool IsGodModeEnabled() const;
};

}

#endif // CONTROLLER_STATES_GAMESCREEN_H__
