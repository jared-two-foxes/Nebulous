
#include "CameraController.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Beta.h>

using namespace Game;


CameraController::CameraController( Nebulae::Camera& camera )
///
/// Default Constructor
///
  : m_camera( &camera )
{}


void 
CameraController::SetVerticalSpeed( const float speed )
{
  m_verticalSpeed = speed;
}


void
CameraController::SetPosition( const Vector4& position )
{
  Vector4 vLookAt( position );
  vLookAt.z -= 500.0f;
  Vector4 vUp( 0.0f, 1.0f, 0.0f );

  m_camera->SetPosition( position );
  m_camera->LookAt( position, vLookAt, vUp );
}


void 
CameraController::SetPosition( const Real depth )
{
  Vector4 vEyePos( 0, depth, 500.0f ); 
  Vector4 vLookAt( 0, depth, 0.0f );
  Vector4 vUp( 0.0f, 1.0f, 0.0f );

  m_camera->SetPosition( vEyePos );
  m_camera->LookAt( vEyePos, vLookAt, vUp );
}


void
CameraController::Update( const float elapsed )
///
/// Move the camera down at the specified pace.
///
/// @return
///   Nothing.
///
{
  const float       dy                  = m_verticalSpeed * elapsed;
  Vector4           cameraPosition      = m_camera->GetPosition();
  Vector4           cameraFocus         = m_camera->GetLookAt();
  cameraPosition.y                     -= dy;
  cameraFocus.y                        -= dy;

  m_camera->SetPosition( cameraPosition );
  m_camera->SetLookAt( cameraFocus );
}