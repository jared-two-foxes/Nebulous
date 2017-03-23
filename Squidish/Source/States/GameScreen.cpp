
#include "GameScreen.h"

#include <Model/Model.h>
#include <Model/Core/Entity.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Component/PlacementConstraintComponent.h>
#include <Model/Component/ScoreComponent.h>
#include <Model/Systems/AudioController.h>

#include <View/EntityRenderSystem.h>
#include <View/View.h>

#include "ViewCtrl/GameScreenViewController.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>

#define INTRO_TIME          3.0f
#define INTRO_CAMERA_SPEED  125.0f
#define GAME_CAMERA_SPEED   100.0f


using namespace Game;

GameScreen::GameScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
  const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
  const ModelPointer& model, const ViewPointer& view, ViewController* viewController ) 
///
/// Constructor.
///
: BaseScreen( "GameScreen", owner, fileSystem,dispatcher, audioController, guiMgr, model, view ),
  m_viewController( viewController ),
  m_motionSystem( model ),
  m_animationSystem( model, view->GetSpriteAnimationManager() ),
  m_collisionSystem( model, view->GetSpriteAtlasManager() ),
  m_intelligenceSystem( model, &m_motionSystem ),
  m_state( STATE_UNKNOWN ),
  m_cameraController( *view->GetCamera() ),
  m_playerController( model, *view->GetCamera() ),
  m_generator( model ),
  m_currentScore( 0 ),
  m_godTime( 0 ),
  m_introTimer( INTRO_TIME )
{
  if( m_viewController != nullptr )
  {
    m_viewController->SetScreen( this );
  }
  
  m_collisionSystem.CollidedSignal.connect( boost::bind(&GameScreen::OnCollision, this, _1, _2) );
}

GameScreen::~GameScreen()
///
/// Destructor
///
{}

GameScreen::State
GameScreen::GetState() const
///
/// Returns the current screen state
///
/// @return
///   The screen state.
///
{ return m_state; }

const MotionSystem& 
GameScreen::GetMotionSystem() const
///
/// Returns the motion system.
///
/// @return
///   The motion system
///
{ return m_motionSystem; }

bool
GameScreen::OnKeyPressed( Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys )
///
/// Callback for keyboard input.  
///
/// @return
///   Nothing.
///
{
  if( !BaseScreen::OnKeyPressed(keyCode, modKeys) && GetState() == STATE_ACTIVE )
  {
    m_playerController.OnKeyPressed( keyCode );
    return true;
  }

  return false;
}

bool
GameScreen::OnKeyReleased( Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys )
///
/// Callback for keyboard input.  
///
/// @return
///   Nothing.
///
{
  if( !BaseScreen::OnKeyReleased(keyCode, modKeys) && GetState() == STATE_ACTIVE )
  {
    m_playerController.OnKeyReleased( keyCode );
    return true;
  }

  return false;
}

bool
GameScreen::OnTouchDown( int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys )
///
/// Callback for "mouse" input.  
///
/// @return
///   Nothing.
///
{ 
  if( !BaseScreen::OnTouchDown(x, y, modKeys) && GetState() == STATE_ACTIVE )
  {
    m_playerController.OnTouchDown( x, y );
    return false;
  }

  return true;
}

bool
GameScreen::OnTouchUp( int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys )
///
/// Callback for "mouse" input.  
///
/// @return
///   Nothing.
///
{
  if( !BaseScreen::OnTouchUp(x, y, modKeys) && GetState() == STATE_ACTIVE )
  {
    m_playerController.OnTouchUp( x, y );
    return true;
  }

  return false;
}

bool
GameScreen::OnTouchDrag( int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys )
///
/// Callback for "mouse" input.  
///
/// @return
///   Nothing.
///
{
  if( BaseScreen::OnTouchDrag(x, y, modKeys) && GetState() == STATE_ACTIVE )
  {
    m_playerController.OnTouchDrag( x, y );
    return true;
  }

  return false;
}

void 
GameScreen::OnUpdate( float elapsed )
///
/// Per loop iteration execution function.
///
/// @param elapsed
///   The amount of time between frames.
///
/// @return
///   Nothing.
///
{
  float depth = 0.0f;

  if( m_state == STATE_INTRO || m_state == STATE_ACTIVE )
  {
  //
  // Update Systems.
  //
    m_cameraController.Update( elapsed );
    m_playerController.Update( elapsed );

    const std::vector<Entity*>& entities = m_model->GetEntities();
    m_motionSystem.Process( entities, elapsed );
    m_animationSystem.Process( entities, elapsed );
    m_collisionSystem.Process( entities, elapsed );
    m_intelligenceSystem.Process( entities, GetPlayerEntity() );

  //
  // Determine the current depth value and update the track generator.
  //
    Nebulae::Camera& camera    = *m_view->GetCamera();
    Vector4          cameraPos = camera.GetPosition();
    depth = cameraPos.y;
    m_generator.Process( depth );

  //
  // Update the Squids constraints.
  //
    NE_ASSERT( m_view != NULL, "The view pointer is invalid." )();

    Nebulae::RenderSystem::WindowPtr window = m_view->GetRenderDevice()->GetWindow();
    const Entity*                    player = GetPlayerEntity();

    NE_ASSERT( player != NULL, "The player pointer is invalid." )();
    NE_ASSERT( window != NULL, "Unable to find a valid window pointer?" )();

    if( player != NULL && window != NULL )
    {
      uint32  width  = window->GetWidth();
      uint32  height = window->GetHeight();

      std::vector<PlacementConstraintComponent* > constraintComponents;
      m_model->GetComponents<PlacementConstraintComponent>( player, &constraintComponents );

      std::for_each( constraintComponents.begin(), constraintComponents.end(), [&](PlacementConstraintComponent* constraint){
        m_playerController.UpdatePlacementConstraint( constraint, width, height );
      });
    }

  //
  // Lock the background to the screen.
  //
    Entity* oceanEntity = m_model->FindEntityByName( "ocean" );
    NE_ASSERT( oceanEntity != NULL, "Unable to find the entity with the name 'ocean'." )();
    if( oceanEntity != NULL ) 
    {
      std::vector<PlacementComponent* > placementComponents;
      m_model->GetComponents<PlacementComponent>( oceanEntity, &placementComponents );

      if( !placementComponents.empty() ) 
      {
        NE_ASSERT( placementComponents.size() == 1, "Expected 1 placement component but found %d", placementComponents.size() )( placementComponents.size() );

        // Prevent the ocean from centering while in the "title" & "intro" sequences
        float oceanDepth = placementComponents[0]->position.y;
        placementComponents[0]->position.y = std::min<float>( depth - 512.0f, oceanDepth );
      }
    }
  }
  
  if( m_state == STATE_INTRO )
  {
    m_introTimer -= elapsed;
    if( m_introTimer <= 0 )
    {
      SetState( STATE_ACTIVE );
    }
    else
    {
      float   factor   = m_introTimer / INTRO_TIME;
      float   strength = -2000.0f * factor * factor;
      Entity* squid    = GetPlayerEntity();
      
      NE_ASSERT( squid, "Unable to find the player Entity." )();
      
      m_motionSystem.ApplySteeringForce( *squid, Vector4( 0, strength, 0, 0 ) );
    }
  }
  else if( m_state == STATE_ACTIVE )
  {
    // Display the current score.
    m_viewController->UpdateScore( m_currentScore );


  //
  // Update the god mode timer.
  // 
    if( m_godTime > 0 ) 
    {
      m_godTime -= elapsed;

      // tint the player while in god mode.
      m_view->ApplyEntityTintFactor( *GetPlayerEntity(), sinf(m_godTime) );

      if( m_godTime <= 0 ) 
      {
        OnGodModeEnded();
      }
    }
  }
}

void
GameScreen::SetViewController( ViewController* viewController )
{
  if( m_viewController != viewController )
  {
    m_viewController = viewController;

    if( m_viewController != nullptr )
    {
      m_viewController->SetScreen( this );
      m_viewController->Init( GetGuiController() );
    }
  }
}


void
GameScreen::SetState( GameScreen::State state ) 
///
/// Changes m_state to @param state 
///
/// @param state
///   The game state to change to.
///
/// @return
///   Nothing.
///
{
  if( state != m_state ) 
  {
    OnStateChanged( state );
    m_state = state;
  } 
}

bool
GameScreen::InvokeAction( const std::string& context )
///
/// Overload for the screen specific action handling.
///
/// @param context
///   The change requested.
///
/// @return
///   Nothing.
///
{
  if( context == "_RESTART_" )
  {
    Restart();
    SetState( STATE_ACTIVE );
    return true;
  }
  else if( context == "_CONTINUE_" )
  {
    Continue();
    return true;
  } 
  
  return false;
}

void
GameScreen::PlayAnimation( Entity& entity, const std::string& animationName )
///
/// Causes an Entity to play a specified animation.
///
/// @param entity
///   The entity to apply the animation to.
///
/// @param animationName
///   The name of the animation to apply.
///
/// @note [jared.watt 31.12.2013]
///   Not sure if this is going to be the most convenient means to implement this or if I should be using
///   some form of entity handle or identifier.
/// 
{
  m_animationSystem.PlayAnimation( entity, animationName );
}

void 
GameScreen::Restart()
///
/// Sets up the game state required to begin the runner.
/// 
/// @return
///   Nothing.
///
{
  Clear();

  m_godTime      = 0;
  m_introTimer   = INTRO_TIME;
  m_currentScore = 0;
  m_playerController.Clear();
  m_generator.Clear();

  SetupScreenFromDefaultParameters();


  Entity* player = GetPlayerEntity();
  NE_ASSERT( player != nullptr, "Unable to find the entity object which represents the user." )();
  m_cameraController.SetPosition( 0.0f );
  m_collisionSystem.SetPlayerEntity( player );
  m_playerController.Init( &m_motionSystem, player );

  std::vector<AspectComponent* > aspectComponents; 
  m_model->GetComponents<AspectComponent>( player, &aspectComponents );    
  NE_ASSERT( aspectComponents.size(), "Unable to find aspect components for player" )();
  aspectComponents[0]->alignment = AspectComponent::BOTTOM;  

  Nebulae::File* file = m_fileSystem->Open( "disk", "track_elements.json" );
  NE_ASSERT( file != nullptr, "Unable to find the file 'track_elements.json'" )();
  if( file != nullptr )
  {
    m_generator.Init( *file );
  }
}

void 
GameScreen::Continue()
///
/// Assuming that the runner progress has paused, function will continue the game
/// progress.
/// 
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_state == STATE_DEAD, "Continue assumes that the user has died." )(); 

//
// Restart
//
  SetState( STATE_ACTIVE );

//
// Enter God Mode.
// 
  m_godTime = 3.0f;
  OnGodModeBegun();
}

void 
GameScreen::OnInit()
{
  //m_audioController->PlayMusic( "music_title", "sounds//ss_01_01.ogg" );
}

void 
GameScreen::OnEnter()
///
/// Setup the game state.
///
/// @return
///   Nothing
///
{
  Restart();
  SetState( STATE_MENU );

  m_viewController->Init( GetGuiController() );

  //m_audioController->ResumeMusic( "music_title" );
}

void
GameScreen::OnExit()
{
  //deinit the view controller.
  //m_audioController->PauseMusic( "music_title" );
}

Entity* 
GameScreen::GetPlayerEntity() const
///
/// Returns the entity which is used as the player character.
/// 
/// @return
///   The player's entity object.
/// 
{
  Entity* player = m_model->FindEntityByName( "squid" );
  NE_ASSERT( player, "Unable to find the squid entity." )();
  return player;
}

void 
GameScreen::OnCollision( const Entity& obj1, const Entity& obj2 )
///
/// Callback hook function that gets invoked when a collision is detected.  
/// 
/// @param obj1
///   The first object in the collision.
///   
/// @param obj2
///   The second object in the collision.
///   
/// @return
///   Nothing.
/// 
{
  NE_ASSERT( GetState() == STATE_ACTIVE, "Expected the game to be in the Active State" )();
  NE_ASSERT( (&obj1 == GetPlayerEntity() || &obj2 == GetPlayerEntity()), "Collision doesn't include the player." )();

  const Entity* player = GetPlayerEntity();
  const Entity* other  = &obj1 != player ? &obj1 : &obj2;

  std::vector<ScoreComponent* > scoreComponents;
  m_model->GetComponents<ScoreComponent>( other, &scoreComponents );

  std::for_each( scoreComponents.begin(), scoreComponents.end(), [&](ScoreComponent* component){
    m_currentScore += component->bonus;
  });

  // Flag unit to be destroyed.
  m_model->DestroyEntity( other );
  
  if( scoreComponents.size() == 0 && !IsGodModeEnabled() )
  {
    //@todo Play the specific dead animation.
    m_audioController->PlaySfx( "sfx_end", "end-1.ogg" );
    SetState( STATE_DEAD );
  }
}

void
GameScreen::OnStateChanged( GameScreen::State state ) 
///
/// Callback function responsible for handling any changes required when changing the game state.  It does not 
/// assume that the state is currently in a specific state, it will respond by changing all elements into the
/// required status when the state is changed.
/// 
/// @param state
///   The new state.
/// 
/// @return
///   Nothing.
///
{
  if( state == STATE_INTRO || state == STATE_ACTIVE )
  {
    m_cameraController.SetVerticalSpeed( state == STATE_INTRO ? INTRO_CAMERA_SPEED : GAME_CAMERA_SPEED );
  }

  m_viewController->StateDidChange( state, GetGuiController() );
  m_motionSystem.SetActive( state == STATE_INTRO || state == STATE_ACTIVE );
}

bool 
GameScreen::IsGodModeEnabled() const
{ return (m_godTime > 0); }

void 
GameScreen::OnGodModeBegun() 
{}

void 
GameScreen::OnGodModeEnded()
{}