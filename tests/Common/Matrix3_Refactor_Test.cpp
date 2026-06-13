#include <Nebulae/Common/Common.h>

#include "gtest/gtest.h"

#include <array>
#include <cmath>

using namespace Nebulae;

namespace
{
constexpr Real kEpsilon = static_cast<Real>( 1e-5 );

void ExpectMatrixNear( const Matrix3& m, const std::array<Real, 9>& expected, Real eps = kEpsilon )
{
  const Real* p = m.ptr();
  for ( size_t i = 0; i < expected.size(); ++i )
  {
    EXPECT_NEAR( p[i], expected[i], eps ) << "Mismatch at linear index " << i;
  }
}
} // namespace


TEST( Matrix3Refactor, Storage_ShouldBeContiguousNineRealValuesInRowMajorOrder )
{
  Matrix3 m( 1, 2, 3, 4, 5, 6, 7, 8, 9 );

  EXPECT_EQ( sizeof( Matrix3 ), sizeof( Real ) * 9 );

  const Real* p = m.ptr();
  EXPECT_EQ( p[0], Real( 1 ) );
  EXPECT_EQ( p[1], Real( 2 ) );
  EXPECT_EQ( p[2], Real( 3 ) );
  EXPECT_EQ( p[3], Real( 4 ) );
  EXPECT_EQ( p[4], Real( 5 ) );
  EXPECT_EQ( p[5], Real( 6 ) );
  EXPECT_EQ( p[6], Real( 7 ) );
  EXPECT_EQ( p[7], Real( 8 ) );
  EXPECT_EQ( p[8], Real( 9 ) );
}


TEST( Matrix3Refactor, Accessors_OperatorIndexAndGetRow_ShouldPreserveRowMajorSemantics )
{
  Matrix3 m( 1, 2, 3, 4, 5, 6, 7, 8, 9 );

  const Vector4 row0 = m.getRow( 0 );
  const Vector4 row1 = m.getRow( 1 );
  const Vector4 row2 = m.getRow( 2 );

  EXPECT_EQ( row0.x, Real( 1 ) );
  EXPECT_EQ( row0.y, Real( 2 ) );
  EXPECT_EQ( row0.z, Real( 3 ) );

  EXPECT_EQ( row1.x, Real( 4 ) );
  EXPECT_EQ( row1.y, Real( 5 ) );
  EXPECT_EQ( row1.z, Real( 6 ) );

  EXPECT_EQ( row2.x, Real( 7 ) );
  EXPECT_EQ( row2.y, Real( 8 ) );
  EXPECT_EQ( row2.z, Real( 9 ) );

  m[1][2] = Real( 42 );

  EXPECT_EQ( m[1][2], Real( 42 ) );
  EXPECT_EQ( m.ptr()[5], Real( 42 ) );
}


TEST( Matrix3Refactor, Accessors_GetColumn_ShouldExtractColumnFromLinearStorage )
{
  Matrix3 m( 1, 2, 3, 4, 5, 6, 7, 8, 9 );

  const Vector4 c0 = m.getColumn( 0 );
  const Vector4 c1 = m.getColumn( 1 );
  const Vector4 c2 = m.getColumn( 2 );

  EXPECT_EQ( c0.x, Real( 1 ) );
  EXPECT_EQ( c0.y, Real( 4 ) );
  EXPECT_EQ( c0.z, Real( 7 ) );

  EXPECT_EQ( c1.x, Real( 2 ) );
  EXPECT_EQ( c1.y, Real( 5 ) );
  EXPECT_EQ( c1.z, Real( 8 ) );

  EXPECT_EQ( c2.x, Real( 3 ) );
  EXPECT_EQ( c2.y, Real( 6 ) );
  EXPECT_EQ( c2.z, Real( 9 ) );
}


TEST( Matrix3Refactor, CopyConstructor_ShouldCopyAllNineElementsIndependently )
{
  Matrix3 original( 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  Matrix3 copied( original );

  original[0][0] = Real( -99 );

  ExpectMatrixNear( copied, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
}


TEST( Matrix3Refactor, AssignmentOperator_ShouldCopyAllNineElementsAndSupportSelfAssignment )
{
  Matrix3 source( 9, 8, 7, 6, 5, 4, 3, 2, 1 );
  Matrix3 assigned;
  assigned = source;

  source[2][2] = Real( -55 );

  ExpectMatrixNear( assigned, { 9, 8, 7, 6, 5, 4, 3, 2, 1 } );

  assigned = assigned;
  ExpectMatrixNear( assigned, { 9, 8, 7, 6, 5, 4, 3, 2, 1 } );
}


TEST( Matrix3Refactor, DeterminantTransposeAdjointInverseAbsolute_ShouldProduceExpectedResults )
{
  const Matrix3 m( 1, 2, 3, 0, 1, 4, 5, 6, 0 );

  EXPECT_NEAR( m.determinant(), Real( 1 ), kEpsilon );

  const Matrix3 transpose = m.transpose();
  ExpectMatrixNear( transpose, { 1, 0, 5, 2, 1, 6, 3, 4, 0 } );

  const Matrix3 adj = m.adjoint();
  ExpectMatrixNear( adj, { -24, 18, 5, 20, -15, -4, -5, 4, 1 } );

  const Matrix3 inv = m.inverse();
  ExpectMatrixNear( inv, { -24, 18, 5, 20, -15, -4, -5, 4, 1 } );

  const Matrix3 neg( -1, 2, -3, 4, -5, 6, -7, 8, -9 );
  const Matrix3 abs = neg.absolute();
  ExpectMatrixNear( abs, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
}


TEST( Matrix3Refactor, Cofac_ShouldUseCorrectElementIndicesWithLinearStorage )
{
  const Matrix3 m( 1, 2, 3, 4, 5, 6, 7, 8, 9 );

  EXPECT_EQ( m.cofac( 0, 0, 1, 1 ), Real( -3 ) ); // 1*5 - 2*4
  EXPECT_EQ( m.cofac( 1, 0, 2, 2 ), Real( -6 ) ); // 4*9 - 6*7
  EXPECT_EQ( m.cofac( 0, 1, 2, 2 ), Real( -6 ) ); // 2*9 - 3*8
}


TEST( Matrix3Refactor, OpenGLSubMatrix_Get_ShouldFillComplete4x4RotationBuffer )
{
  Matrix3 m( 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  Real gl[16];
  for ( Real& v : gl )
  {
    v = Real( -99 );
  }

  m.getOpenGLSubMatrix( gl );

  EXPECT_EQ( gl[0], Real( 1 ) );
  EXPECT_EQ( gl[1], Real( 4 ) );
  EXPECT_EQ( gl[2], Real( 7 ) );
  EXPECT_EQ( gl[3], Real( 0 ) );

  EXPECT_EQ( gl[4], Real( 2 ) );
  EXPECT_EQ( gl[5], Real( 5 ) );
  EXPECT_EQ( gl[6], Real( 8 ) );
  EXPECT_EQ( gl[7], Real( 0 ) );

  EXPECT_EQ( gl[8], Real( 3 ) );
  EXPECT_EQ( gl[9], Real( 6 ) );
  EXPECT_EQ( gl[10], Real( 9 ) );
  EXPECT_EQ( gl[11], Real( 0 ) );

  EXPECT_EQ( gl[12], Real( 0 ) );
  EXPECT_EQ( gl[13], Real( 0 ) );
  EXPECT_EQ( gl[14], Real( 0 ) );
  EXPECT_EQ( gl[15], Real( 1 ) );
}


TEST( Matrix3Refactor, OpenGLSubMatrix_Set_ShouldMapColumnMajorInputToRowMajorStorage )
{
  const Real gl[16] = {
    1, 4, 7, 101, 2, 5, 8, 102, 3, 6, 9, 103, 201, 202, 203, 204,
  };

  Matrix3 m;
  m.setFromOpenGLSubMatrix( gl );

  ExpectMatrixNear( m, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
}


TEST( Matrix3Refactor, Operators_AddSubtractMultiplyAndEquality_ShouldWorkWithLinearStorage )
{
  const Matrix3 a( 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  const Matrix3 b( 9, 8, 7, 6, 5, 4, 3, 2, 1 );

  const Matrix3 sum = a + b;
  ExpectMatrixNear( sum, { 10, 10, 10, 10, 10, 10, 10, 10, 10 } );

  const Matrix3 diff = a - b;
  ExpectMatrixNear( diff, { -8, -6, -4, -2, 0, 2, 4, 6, 8 } );

  const Matrix3 prod = a * b;
  ExpectMatrixNear( prod, { 30, 24, 18, 84, 69, 54, 138, 114, 90 } );

  const Matrix3 scaled = a * Real( 2 );
  ExpectMatrixNear( scaled, { 2, 4, 6, 8, 10, 12, 14, 16, 18 } );

  EXPECT_TRUE( a == a );
  EXPECT_FALSE( a == b );
}


TEST( Matrix3Refactor, GetRotation_ShouldRoundTripQuaternionUsingCorrectElementAccess )
{
  const Real pi = static_cast<Real>( std::acos( -1.0 ) );
  Quaternion input( Vector4( 0, 1, 0 ), pi / Real( 3 ) );
  Matrix3 m( input );

  Quaternion output;
  m.getRotation( output );

  Quaternion a = input.normalized();
  Quaternion b = output.normalized();
  const Real alignment = std::fabs( a.dot( b ) );

  EXPECT_NEAR( alignment, Real( 1 ), static_cast<Real>( 1e-4 ) );
}
