
#include "PlayerController.h"

#include <Model/Model.h>
#include <Model/Core/Entity.h>
#include <Model/Component/ParticleComponent.h>
#include <Model/Component/PlacementConstraintComponent.h>
#include <Model/Systems/MotionSystem.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Beta.h>

using namespace Game;

  
PlayerController::PlayerController( const ModelPtr& model, Nebulae::Camera& camera )
///
/// Default Constructor
///
: m_model( model ),
  m_motionSystem( NULL ),
  m_player( NULL ),
  m_camera( camera ),
  m_pulseTimeRemaining( 0.f ),
  m_moveLeft( false ),
  m_moveRight( false )
{}

void 
PlayerController::Clear() 
{
  m_motionSystem       = NULL;
  m_player             = NULL;
  m_moveLeft           = false;
  m_moveRight          = false;
  m_pulseTimeRemaining = 0.0f;
}

void
PlayerController::Init( MotionSystem* motionSystem, Entity* player )
{
  m_motionSystem = motionSystem;
  m_player       = player;
}

void
PlayerController::OnKeyPressed( Nebulae::KeyCode keyCode ) 
{
  switch( keyCode ) {
    case Nebulae::VKC_LEFT:
      m_moveLeft = true;
      break;
    case Nebulae::VKC_RIGHT:
      m_moveRight = true;
      break;
  }
}

void
PlayerController::OnKeyReleased( Nebulae::KeyCode keyCode ) 
{
  switch( keyCode ) {
    case Nebulae::VKC_LEFT:
      m_moveLeft = false;
      break;
    case Nebulae::VKC_RIGHT:
      m_moveRight = false;
      break;
  }
}

void
PlayerController::OnTouchDown( int x, int y ) 
{}

void
PlayerController::OnTouchUp( int x, int y )
{
  m_pulseTimeRemaining += 2.0f;

  // Add a new "ink squirt" particle.
  ParticleComponent* particleComponent = m_model->AddComponentToEntity<ParticleComponent>( m_player );
  particleComponent->emitterName = "particles//ink.emitter";
  //@todo seperate the positioning of this particle from the entities PlacementComponent.  They are currently
  //      tied.
}

void
PlayerController::OnTouchDrag( int x, int y )
{}

void
PlayerController::Update( float elapsed )
///
/// Updates the position of the player based upon user input and game state.
///
/// @param elapsed
///   Time that has expired between frames.
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_player, "Expected the player object to be created." )();

  const Real    constHorizSpeed    = 160.0f;
  const Real    constVerticalSpeed = 350.0f;
  const Real    neutralDepth       = 180.0f;
  const Vector4 cameraDepth        = m_camera.GetLookAt();
  Vector4       position           = m_motionSystem->GetPosition( m_model, *m_player );  
  position.z = 0;

//
// Determine strafe influence.
//  
  Vector4 strafeOffset  = Vector4(0,0,0,0);
  if( m_moveRight ) {
    strafeOffset += Vector4( constHorizSpeed, 0, 0 );
  } else if( m_moveLeft ) {
    strafeOffset -= Vector4( constHorizSpeed, 0, 0 );
  }

//
// Determine pulse influence.
//
  Vector4 pulseOffset(0,0,0,0);
  if( m_pulseTimeRemaining > 0.0f ) {
    pulseOffset.y        -= constVerticalSpeed * m_pulseTimeRemaining;
    m_pulseTimeRemaining -= elapsed;
  }

//
// Determine the desired position.
//
  Vector4 actualNeutralDepth  = cameraDepth + Vector4( position.x, neutralDepth, 0 );
  Vector4 desiredTarget       = actualNeutralDepth;
  desiredTarget              += strafeOffset;
  desiredTarget              += pulseOffset;

//
// Determine the steering force required to move towards the desired position.
//
  const Vector4 desiredVelocity = desiredTarget - position;
  const Vector4 currentVelocity = m_motionSystem->GetVelocity( m_model, *m_player );
  const Vector4 steeringForce   = desiredVelocity - currentVelocity;
  
  m_motionSystem->ApplySteeringForce( *m_player, steeringForce );
}

void 
PlayerController::UpdatePlacementConstraint( PlacementConstraintComponent* placementConstraintComponent, uint32 width, uint32 height )
{
  const Vector4 cameraPosition = m_camera.GetLookAt();
  const float   halfHeight     = height * 0.5f;
  const float   halfWidth      = width * 0.5f;

  if( placementConstraintComponent != NULL ) {
    placementConstraintComponent->leftBound   = -halfHeight;
    placementConstraintComponent->rightBound  = halfHeight;
    placementConstraintComponent->bottomBound = cameraPosition.y + halfHeight; 
    placementConstraintComponent->topBound    = cameraPosition.y - halfHeight;
  }
}