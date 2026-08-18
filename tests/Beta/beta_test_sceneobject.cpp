#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

#include <cstdlib>
#include <fstream>
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

template <typename T, typename = void> struct HasKeyedAddProvider : std::false_type
{
};

template <typename T>
struct HasKeyedAddProvider<T, std::void_t<decltype( std::declval<T&>().AddProvider(
                                std::declval<const std::string&>(), std::declval<UniformProvider>() ) )>>
  : std::true_type
{
};

template <typename T> void AssertProviderReplacement( T& object )
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

template <typename T, typename = void> struct HasEmitDrawItems : std::false_type
{
};

template <typename T>
struct HasEmitDrawItems<
  T, std::void_t<decltype( std::declval<T&>().EmitDrawItems( std::declval<DrawItemList&>(), 0, 0 ) )>> : std::true_type
{
};

template <typename T> void AssertDrawItemEmission( T& object, DrawItemList& items )
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

template <typename T, typename = void> struct HasLegacyMaterialAccessor : std::false_type
{
};

template <typename T>
struct HasLegacyMaterialAccessor<T, std::void_t<decltype( std::declval<const T&>().GetMaterial() )>>
  : std::true_type
{
};

template <typename T, typename = void> struct HasLegacyInitialization : std::false_type
{
};

template <typename T>
struct HasLegacyInitialization<T, std::void_t<decltype( std::declval<T&>().Initialize() )>> : std::true_type
{
};

template <typename T, typename = void> struct HasLegacyGeometrySetter : std::false_type
{
};

template <typename T>
struct HasLegacyGeometrySetter<
  T, std::void_t<decltype( std::declval<T&>().SetGeometry( std::declval<std::size_t>(), std::declval<Geometry*>() ) )>>
  : std::true_type
{
};

template <typename T, typename = void> struct HasLegacyInputLayoutSetter : std::false_type
{
};

template <typename T>
struct HasLegacyInputLayoutSetter<T,
                                  std::void_t<decltype( std::declval<T&>().SetInputLayout(
                                    std::declval<std::size_t>(), std::declval<InputLayout*>() ) )>> : std::true_type
{
};

template <typename T, typename = void> struct HasLegacyClear : std::false_type
{
};

template <typename T>
struct HasLegacyClear<T, std::void_t<decltype( std::declval<T&>().Clear() )>> : std::true_type
{
};

std::string ReadSceneObjectSource( const std::string& relativePath )
{
  std::string sourcePath = relativePath;
  const char* runfilesDir = std::getenv( "RUNFILES_DIR" );
  if ( !runfilesDir )
    runfilesDir = std::getenv( "TEST_SRCDIR" );
  if ( runfilesDir )
    sourcePath = std::string( runfilesDir ) + "/" + relativePath;

  std::ifstream source( sourcePath, std::ios::binary );
  if ( !source.is_open() )
  {
    const char* manifestPath = std::getenv( "RUNFILES_MANIFEST_FILE" );
    if ( manifestPath )
    {
      std::ifstream manifest( manifestPath );
      std::string logicalPath;
      while ( std::getline( manifest, logicalPath ) )
      {
        const std::string::size_type separator = logicalPath.find( ' ' );
        if ( separator == std::string::npos )
          continue;
        const std::string realPath = logicalPath.substr( separator + 1 );
        logicalPath.resize( separator );
        if ( logicalPath.size() >= relativePath.size() &&
             logicalPath.compare( logicalPath.size() - relativePath.size(), relativePath.size(), relativePath ) == 0 )
        {
          source.clear();
          source.open( realPath, std::ios::binary );
          break;
        }
      }
    }
  }

  if ( !source.is_open() )
    return std::string();
  return std::string( ( std::istreambuf_iterator<char>( source ) ), std::istreambuf_iterator<char>() );
}

TEST( SceneObject, DoesNotExposeLegacyMaterialOrPassInitializationState )
{
  EXPECT_FALSE( HasLegacyMaterialAccessor<SceneObject>::value );
  EXPECT_FALSE( HasLegacyInitialization<SceneObject>::value );
  EXPECT_FALSE( HasLegacyGeometrySetter<SceneObject>::value );
  EXPECT_FALSE( HasLegacyInputLayoutSetter<SceneObject>::value );
  EXPECT_FALSE( HasLegacyClear<SceneObject>::value );

  // The obsolete state is private, so also inspect both declarations and
  // definitions rather than relying only on the public API surface.
  const std::string header = ReadSceneObjectSource( "Source/Nebulae/Beta/Scene/SceneObject.h" );
  const std::string implementation = ReadSceneObjectSource( "Source/Nebulae/Beta/Scene/SceneObject.cpp" );
  ASSERT_FALSE( header.empty() );
  ASSERT_FALSE( implementation.empty() );
  EXPECT_EQ( std::string::npos, header.find( "PassData" ) );
  EXPECT_EQ( std::string::npos, header.find( "m_material" ) );
  EXPECT_EQ( std::string::npos, header.find( "m_passData" ) );
  EXPECT_EQ( std::string::npos, implementation.find( "PassData" ) );
  EXPECT_EQ( std::string::npos, implementation.find( "m_material" ) );
  EXPECT_EQ( std::string::npos, implementation.find( "m_passData" ) );

  // SceneNode must now build the slot-based object directly; it must not rely
  // on the removed material/pass initialization state.
  SceneNode node( nullptr );
  Material material( "scene_material" );
  SceneObject* object = node.CreateObject( &material );

  ASSERT_NE( nullptr, object );
  ASSERT_EQ( std::size_t( 1 ), object->GetSlotCount() );
  EXPECT_EQ( &material, object->GetSlot( 0 ).material );
  EXPECT_EQ( object, node.FindSubObject( &material ) );
}
