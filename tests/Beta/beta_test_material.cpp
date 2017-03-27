
#include <Nebulae/Beta/Material/Material.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(Material, CreatePass_ShouldIncreasePassCountBy1) {
  // arrange
  Material* material = new Material( "material_01" );

  //act
  material->CreatePass();
  
  //assert
  ASSERT_EQ( (std::size_t)1, material->GetPassCount() );
}

TEST(Material, CreatePass_ShouldReturnAPassPointer) {
  // arrange
  Material* material = new Material( "material_01" );

  //act
  Pass* pass = material->CreatePass();
  
  //assert
  ASSERT_TRUE( pass != NULL );
}

TEST(Material, RemovePass_ValidIndex_ShouldReducePassCountBy1) {
  // arrange
  Material* material = new Material( "material_01" );
  Pass*     pass     = material->CreatePass();

  //act
  material->RemovePass( 0 );
  
  //assert
  ASSERT_EQ( std::size_t(0), material->GetPassCount() );
}

TEST(Material, RemovePass_InvalidIndex_ShouldNotReducePassCount) {
  // arrange
  Material*   material  = new Material( "material_01" );
  Pass*       pass      = material->CreatePass();
  std::size_t passCount = material->GetPassCount();

  //act
  material->RemovePass( -1 );
  
  //assert
  ASSERT_EQ( passCount, material->GetPassCount() );
}

TEST(Material, RemovePass_IndexOutOfBounds_ShouldNotReducePassCount) {
  // arrange
  Material*   material  = new Material( "material_01" );
  Pass*       pass      = material->CreatePass();
  std::size_t passCount = material->GetPassCount();

  //act
  material->RemovePass( 2 );
  
  //assert
  ASSERT_EQ( passCount, material->GetPassCount() );
}