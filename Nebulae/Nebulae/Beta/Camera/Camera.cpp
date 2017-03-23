
#include "Camera.h"

#include <Nebulae/Common/Window/Window.h>


using namespace Nebulae;


Camera::Camera()
: m_position( Vector4( 0, 0, 0 ) ), 
  m_lookAt( Vector4( 0, 0, 0 ) ), 
  m_up( Vector4( 0, 1, 0 ) ), 
  m_view( Matrix4::Identity ),
  m_projection( Matrix4::Identity )
{}

const Vector4&
Camera::GetPosition() const
{ return m_position; }

const Vector4&
Camera::GetLookAt() const
{ return m_lookAt; }

const Vector4&
Camera::GetUp() const
{ return m_up; }

const Matrix4&
Camera::GetViewMatrix() const
{ return m_view; }

const Matrix4&
Camera::GetProjectionMatrix() const
{ return m_projection; }

void 
Camera::SetPosition( const Vector4& pos )
{
	m_position = pos;
	LookAt( m_position, m_lookAt, m_up );
}

void 
Camera::SetLookAt( const Vector4& at )
{
	m_lookAt = at;
	LookAt( m_position, m_lookAt, m_up );
}

void
Camera::SetPerspective( const Real fFovY, const Real fAspectRatio, const Real near_, const Real far_ )
{
  //const Real height = tanf( M_PI_2 - (fFovY*0.5f) );
  //const Real width  = height * fAspectRatio;
  //const Real right  = 0.5f*width;
  //const Real top    = 0.5f*height;

  //memset( &m_fProjection[0], 0, 16*sizeof(Real) );

  //m_fProjection[0]  = near_ / right;
  //m_fProjection[5]  = near_ / top;
  //m_fProjection[10] = ( - ( far_ + near_ ) ) / ( far_ - near_ );
  //m_fProjection[11] = -1.0f;
  //m_fProjection[14] = ( -2.0f * far_ * near_ ) / ( far_ - near_ );
}

void 
Camera::SetOrtho( const Real width, const Real height, const Real near_, const Real far_ )
{
	const Real left = -0.5f*width, right = 0.5f*width, bottom = -0.5f*height, top = 0.5f*height;
  
  m_projection.MakeOrthoMatrix( left, right, bottom, top, near_, far_ );
}

void 
Camera::LookAt( const Vector4& vEyePosition, const Vector4& vFocusPosition, const Vector4& vUpDirection )
{
	m_position = vEyePosition;
	m_lookAt   = vFocusPosition;
	m_up       = vUpDirection;

  m_view.MakeLookAtMatrix( m_position, m_lookAt, m_up );
}