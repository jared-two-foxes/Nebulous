
#include <Nebulae/Common/Common.h>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace Nebulae;

const Matrix4 Matrix4::Identity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

const Matrix4 Matrix4::Zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


void Matrix4::MakeOrthoMatrix( const Real LEFT, const Real RIGHT, const Real BOTTOM, const Real TOP, const Real near_,
                               const Real far_ )
{
  m[0] = static_cast<Real>( 2.F ) / ( RIGHT - LEFT );
  m[1] = 0.0F;
  m[2] = 0.0F;
  m[3] = -( ( RIGHT + LEFT ) / ( RIGHT - LEFT ) );

  m[4] = 0.0F;
  m[5] = static_cast<Real>( 2.F ) / ( TOP - BOTTOM );
  m[6] = 0.0F;
  m[7] = -( ( TOP + BOTTOM ) / ( TOP - BOTTOM ) );

  m[8] = 0.0F;
  m[9] = 0.0F;
  m[10] = -static_cast<Real>( 2.F ) / ( far_ - near_ );
  m[11] = -( ( far_ + near_ ) / ( far_ - near_ ) );

  m[12] = 0.0F;
  m[13] = 0.0F;
  m[14] = 0.0F;
  m[15] = static_cast<Real>( 1.F );
}

void Matrix4::MakePerspectiveMatrix( const Real FOV, const Real NEARPLANE, const Real FARPLANE, const Real ASPECT )
{
  Real f = static_cast<Real>( 1.F ) / tanf( FOV / 2.0F );

  m[0] = f / ASPECT;
  m[1] = 0.0F;
  m[2] = 0.0F;
  m[3] = 0.0F;

  m[4] = 0.0F;
  m[5] = f;
  m[6] = 0.0F;
  m[7] = 0.0F;

  m[8] = 0.0F;
  m[9] = 0.0F;
  m[10] = ( FARPLANE + NEARPLANE ) / ( NEARPLANE - FARPLANE );
  m[11] = ( 2.0F * FARPLANE * NEARPLANE ) / ( NEARPLANE - FARPLANE );

  m[12] = 0.0F;
  m[13] = 0.0F;
  m[14] = -1.0F;
  m[15] = 0.0F;
}

void Matrix4::MakePerspectiveMatrix( const Real L, const Real R, const Real T, const Real B, const Real N,
                                     const Real F )
{
}

void Matrix4::MakeLookAtMatrix( const Vector4& pos, const Vector4& at, const Vector4& up )
{
  Vector4 zaxis = at - pos;
  zaxis.normalize();

  Vector4 xaxis = zaxis.cross( up );
  xaxis.normalize();

  Vector4 yaxis = xaxis.cross( zaxis );
  yaxis.normalize();

  m[0] = xaxis.x;
  m[1] = xaxis.y;
  m[2] = xaxis.z;
  m[3] = -xaxis.dot( pos );
  m[4] = yaxis.x;
  m[5] = yaxis.y;
  m[6] = yaxis.z;
  m[7] = -yaxis.dot( pos );
  m[8] = -zaxis.x;
  m[9] = -zaxis.y;
  m[10] = -zaxis.z;
  m[11] = zaxis.dot( pos );
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = static_cast<Real>( 1.F );
}
