
#include <Nebulae/Common/Common.h>

using namespace Nebulae;

const Matrix4 Matrix4::Identity =
{ 1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1 };

const Matrix4 Matrix4::Zero =
{ 0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0 };



void 
Matrix4::MakeOrthoMatrix( const Real left, const Real right, const Real bottom, const Real top, const Real near_, const Real far_ )
{ 
  m[0]  = Real( 2.f ) / ( right - left );
  m[1]  = 0.0f;
  m[2]  = 0.0f;
	m[3]  = - (( right + left ) / ( right - left ));

  m[4]  = 0.0f;
	m[5]  = Real( 2.f ) / ( top - bottom );
  m[6]  = 0.0f;
	m[7]  = - (( top + bottom ) / ( top - bottom ));

  m[8]  = 0.0f;
  m[9]  = 0.0f;
	m[10] = - Real( 2.f ) / ( far_ - near_ );
	m[11] = - (( far_ + near_ ) / ( far_ - near_ ));

  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = Real( 1.f );
}

void 
Matrix4::MakePerspectiveMatrix( const Real fov, const Real nearplane, const Real farplane, const Real aspect )
{}

void 
Matrix4::MakePerspectiveMatrix( const Real l, const Real r, const Real t, const Real b, const Real n, const Real f )
{}

void 
Matrix4::MakeLookAtMatrix( const Vector4& pos, const Vector4& at, const Vector4& up )
{
	Vector4 zaxis = at - pos;
	zaxis.normalize();

	Vector4 xaxis = zaxis.cross( up );
	xaxis.normalize();

  Vector4 yaxis = xaxis.cross( zaxis );
	yaxis.normalize();

	m[0]  = xaxis.x;
	m[1]  = xaxis.y;
	m[2]  = xaxis.z;
	m[3]  = - xaxis.dot( pos );
	m[4]  = yaxis.x;
	m[5]  = yaxis.y;
	m[6]  = yaxis.z;
	m[7]  = - yaxis.dot( pos );
	m[8]  = - zaxis.x;
	m[9]  = - zaxis.y;
	m[10] = - zaxis.z;
	m[11] = zaxis.dot( pos );
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = Real( 1.f );
}