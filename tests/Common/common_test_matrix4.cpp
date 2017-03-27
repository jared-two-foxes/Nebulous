
#include <Nebulae/Common/Common.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(Matrix4, EqualsOperator_EquivalentMatrices_ShouldReturnTrue) {
  //arrange
  Matrix4 m(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

  //act
  bool result = (m == m);

  ASSERT_TRUE( result );
}


TEST(Matrix4, EqualsOperator_DifferentMatrices_ShouldReturnFalse) {
  //arrange
  Matrix4 m1(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
  Matrix4 m2(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);

  //act
  bool result = (m1 == m2);

  ASSERT_FALSE( result );
}


TEST(Matrix4, NotEqualsOperator_DifferentMatrices_ShouldReturnTrue) {
  //arrange
  Matrix4 m1(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
  Matrix4 m2(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);

  //act
  bool result = (m1 != m2);

  ASSERT_TRUE( result );
}


TEST(Matrix4, NotEqualsOperator_EquivalentMatrices_ShouldReturnFalse) {
  //arrange
  Matrix4 m(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

  //act
  bool result = (m != m);

  ASSERT_FALSE( result );
}


TEST(Matrix4, AddOperator_ShouldReturnValidResult) {
  // arrange
  Matrix4 m1( 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1 );
  Matrix4 m2( 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2 );
  Matrix4 expected( 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3 );

  // act  
  Matrix4 m = m1 + m2;

  ASSERT_TRUE( m == expected );
}


TEST(Matrix4, SetIdentity_ShouldCreateIdentityMatrix) {
  // arrange
  const Matrix4 r(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
  Matrix4 m(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

  // act
  m.SetIdentity();

  ASSERT_EQ( m, r );
}