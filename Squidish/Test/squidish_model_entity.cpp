
#include "Model/Model.h"
#include "Model/Core/Entity.h"

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


TEST(Entity, GetLinkedComponentIndices_HasNoComponentsOfSpecifiedArguments_ReturnsCount) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 1 );

  //act
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_ASPECT, NULL );

  //assert
  ASSERT_EQ( count, 0 );
}

TEST(Entity, GetLinkedComponentIndices_NoComponentsOfType_ReturnsZero) 
{
  //arrange
  Entity entity( 0 );

  //act
  std::vector<int32 > indices;
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, &indices );

  //assert
  ASSERT_EQ( count, 0 );
}

TEST(Entity, GetLinkedComponentIndices_HasComponents_ReturnsSizeOfArray) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 1 );

  //act
  std::vector<int32 > indices;
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, &indices );

  //assert
  ASSERT_EQ( indices.size(), count );
}

TEST(Entity, GetLinkedComponentIndices_NoComponentsOfTypeAndNUllPointer_ReturnsZero) 
{
  //arrange
  Entity entity( 0 );

  //act
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, NULL );

  //asserts
  ASSERT_EQ( count, 0 );
}

TEST(Entity, GetLinkedComponentIndices_HasComponents_ReturnsArrayWithCorrectComponentCount) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 1 );

  //act
  std::vector<int32 > indices;
  entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, &indices );

  //asserts
  ASSERT_EQ( indices.size(), 2 );
}

TEST(Entity, GetLinkedComponentIndices_MultipleComponentTypes_ReturnsCorrectComponentCount) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );
  entity.AddComponent( COMPONENT_TYPE_ASPECT, 0 );

  //act
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, NULL );

  //asserts
  ASSERT_EQ( count, 1 );
}

TEST(Entity, GetLinkedComponentIndices_NullIndexPointer_ReturnsCorrectComponentCount) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 1 );

  //act
  int32 count = entity.GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, NULL );

  //asserts
  ASSERT_EQ( count, 2 );
}

TEST(Entity, AddComponent_DuplicateIndices_ReturnsFalse) 
{
  //arrange
  Entity entity( 0 );
  entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );

  //act
  bool success = entity.AddComponent( COMPONENT_TYPE_PLACEMENT, 0 );

  //asserts
  ASSERT_TRUE( !success );
}