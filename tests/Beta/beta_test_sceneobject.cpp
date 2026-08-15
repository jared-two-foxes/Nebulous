#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

#include "gtest/gtest.h"

using namespace Nebulae;

TEST( SceneObject, AddSlot_ReturnsZeroBasedIndexAndStoresMaterial )
{
  SceneObject object( nullptr );
  Material material( "material_01" );

  const std::size_t slotIndex = object.AddSlot( &material );

  ASSERT_EQ( std::size_t( 0 ), slotIndex );
  ASSERT_EQ( std::size_t( 1 ), object.GetSlotCount() );
  EXPECT_EQ( &material, object.GetSlot( slotIndex ).material );
}
