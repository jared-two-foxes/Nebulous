#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

#include <string>
#include <type_traits>
#include <utility>

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

template <typename T, typename = void>
struct HasKeyedAddProvider : std::false_type
{
};

template <typename T>
struct HasKeyedAddProvider<
  T,
  std::void_t<decltype( std::declval<T&>().AddProvider(
    std::declval<const std::string&>(), std::declval<UniformProvider>() ) )>>
  : std::true_type
{
};

template <typename T>
void AssertProviderReplacement( T& object )
{
  if constexpr ( HasKeyedAddProvider<T>::value )
  {
    bool firstCalled = false;
    bool replacementCalled = false;
    bool otherCalled = false;

    UniformProvider first = [&]( UniformBinder& ) { firstCalled = true; };
    UniformProvider replacement = [&]( UniformBinder& ) { replacementCalled = true; };
    UniformProvider other = [&]( UniformBinder& ) { otherCalled = true; };

    object.AddProvider( "sprite", first );
    object.AddProvider( "sprite", replacement );
    object.AddProvider( "other", other );

    const auto& providers = object.GetSlot( 0 ).providers;
    ASSERT_EQ( std::size_t( 2 ), providers.size() );

    UniformBinder binder;
    bool foundSprite = false;
    bool foundOther = false;
    for ( const auto& provider : providers )
    {
      if ( provider.first == "sprite" )
      {
        foundSprite = true;
        provider.second( binder );
      }
      else if ( provider.first == "other" )
      {
        foundOther = true;
        provider.second( binder );
      }
    }

    EXPECT_TRUE( foundSprite );
    EXPECT_TRUE( foundOther );
    EXPECT_FALSE( firstCalled );
    EXPECT_TRUE( replacementCalled );
    EXPECT_TRUE( otherCalled );
  }
  else
  {
    ADD_FAILURE() << "SceneObject does not expose keyed AddProvider";
  }
}

TEST( SceneObject, AddProvider_WithExistingKey_ReplacesProvider )
{
  SceneObject object( nullptr );
  object.AddSlot( nullptr );

  AssertProviderReplacement( object );
}

template <typename T, typename = void>
struct HasEmitDrawItems : std::false_type
{
};

template <typename T>
struct HasEmitDrawItems<
  T,
  std::void_t<decltype( std::declval<T&>().EmitDrawItems(
    std::declval<DrawItemList&>(), 0, 0 ) )>>
  : std::true_type
{
};

template <typename T>
void AssertDrawItemEmission( T& object, DrawItemList& items )
{
  if constexpr ( HasEmitDrawItems<T>::value )
  {
    object.EmitDrawItems( items, 0, 0 );
    EXPECT_EQ( std::size_t( 3 ), items.Size() );
  }
  else
  {
    ADD_FAILURE() << "SceneObject does not expose EmitDrawItems";
  }
}

TEST( SceneObject, EmitDrawItems_EmitsOneItemPerSlotAndMaterialPass )
{
  SceneObject object( nullptr );
  Material onePass( "one_pass" );
  Material twoPass( "two_pass" );
  onePass.CreatePass();
  twoPass.CreatePass();
  twoPass.CreatePass();
  object.AddSlot( &onePass );
  object.AddSlot( &twoPass );

  DrawItemList items;
  AssertDrawItemEmission( object, items );
}
