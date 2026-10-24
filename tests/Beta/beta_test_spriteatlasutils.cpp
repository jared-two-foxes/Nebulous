#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/FileSystem.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>

#include <Mock/MockRenderSystem.h>
#include <Mock/MockWindow.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace Nebulae;

using ::testing::NiceMock;

namespace
{
class TestTextureImpl : public TextureImpl
{
public:
  TestTextureImpl( const std::string& name, std::size_t width, std::size_t height ) : TextureImpl( name )
  {
    m_width = width;
    m_height = height;
  }
};

class TestInputLayoutImpl : public InputLayoutImpl
{
public:
  using InputLayoutImpl::InputLayoutImpl;

  bool Load() override { return true; }
  bool Unload() override { return true; }
};

class SpriteAtlasUtilsFixture : public ::testing::Test
{
protected:
  std::shared_ptr<FileSystem> fileSystem;
  std::shared_ptr<Window> window;
  std::shared_ptr<MockRenderDevice> device;

  void SetUp() override
  {
    fileSystem = std::shared_ptr<FileSystem>( new FileSystem() );
    fileSystem->Mount( "disk", new DiskFileDevice( "../../tests/Assets" ) );
    window = std::shared_ptr<MockWindow>( new MockWindow() );
    device = std::shared_ptr<MockRenderDevice>( new NiceMock<MockRenderDevice>( fileSystem, window ) );
    ASSERT_TRUE( device->Initiate() );
  }
};
} // namespace

TEST_F( SpriteAtlasUtilsFixture, SetSpriteFrame_RegistersOneProviderForRepeatedUpdates )
{
  // Register the texture with the renderer so SpriteAtlas::Load() can find it.
  // false prevents an attempt to load an actual image file.
  Texture* texture = device->CreateTexture( "atlas_texture", false );
  ASSERT_NE( nullptr, texture );
  texture->SetImpl( new TestTextureImpl( "atlas_texture", 64, 64 ) );

  SpriteAtlas atlas( "test_atlas", device );

  std::unique_ptr<File> stream( fileSystem->Open( "disk", "spriteatlas_test.json" ) );
  ASSERT_NE( nullptr, stream.get() );
  const bool loaded = atlas.Load( stream.get() );
  ASSERT_TRUE( loaded );

  // Establish fixture validity before testing SetSpriteFrame().
  ASSERT_NE( nullptr, atlas.FindModuleSubTexture( "frame_a" ) );
  ASSERT_NE( nullptr, atlas.FindModuleSubTexture( "frame_b" ) );

  ON_CALL( *device, CreateInputLayoutImpl( testing::_, testing::_ ) )
    .WillByDefault( []( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader ) -> InputLayoutImpl*
                    { return new TestInputLayoutImpl( pVertexDecl, pVertexShader ); } );
  HardwareShader vertexShader( "sprite_test_vs", device.get() );

  Material material( "sprite_material" );
  material.CreatePass()->SetVertexShader( &vertexShader );
  SceneObject object( nullptr );
  object.AddSlot( &material );

  SpriteAtlasUtils::SetSpriteFrame( device, &material, &object, &atlas, "frame_a" );

  ASSERT_EQ( std::size_t( 1 ), object.GetSlot( 0 ).providers.size() );
  ASSERT_EQ( "sprite_frame", object.GetSlot( 0 ).providers.front().first );

  const Geometry* firstGeometry = object.GetSlot( 0 ).geometry;
  const InputLayout* firstInputLayout = object.GetSlot( 0 ).inputLayout;

  ASSERT_NE( nullptr, firstGeometry );
  ASSERT_NE( nullptr, firstGeometry->m_vertexDeceleration );
  ASSERT_NE( nullptr, firstInputLayout );

  const VertexDeceleration* firstVertexDecl = firstGeometry->m_vertexDeceleration;

  // Keep the original callable to verify that it observes later state updates.
  UniformProvider firstProvider = object.GetSlot( 0 ).providers.front().second;

  // Instrument the existing provider. Frame updates must leave it registered.
  int providerInvocations = 0;
  object.AddProvider( "sprite_frame",
                      [firstProvider, &providerInvocations]( UniformBinder& binder )
                      {
                        ++providerInvocations;
                        firstProvider( binder );
                      } );

  SpriteAtlasUtils::SetSpriteFrame( device, &material, &object, &atlas, "frame_b" );

  ASSERT_EQ( std::size_t( 1 ), object.GetSlot( 0 ).providers.size() );
  EXPECT_EQ( "sprite_frame", object.GetSlot( 0 ).providers.front().first );
  ASSERT_NE( nullptr, object.GetSlot( 0 ).geometry );
  EXPECT_EQ( firstGeometry, object.GetSlot( 0 ).geometry ) << "Frame updates must reuse the existing geometry.";
  EXPECT_EQ( firstVertexDecl, object.GetSlot( 0 ).geometry->m_vertexDeceleration )
    << "Frame updates must reuse the existing vertex declaration.";
  EXPECT_EQ( firstInputLayout, object.GetSlot( 0 ).inputLayout )
    << "Frame updates must reuse the existing input layout.";
  // If SetSpriteFrame re-registers the provider, our wrapper is lost.
  UniformBinder registeredBinder;
  object.GetSlot( 0 ).providers.front().second( registeredBinder );
  EXPECT_EQ( 1, providerInvocations ) << "SetSpriteFrame must not replace the registered sprite provider.";

  // The original provider must observe the updated state.  This distinguishes
  // one stable registration from replacement by a new frame-capturing lambda.
  UniformBinder binder;
  firstProvider( binder );

  const UniformBinding* sizeBinding = nullptr;
  const UniformBinding* minUvBinding = nullptr;
  const UniformBinding* maxUvBinding = nullptr;
  for ( const UniformBinding& binding : binder.GetBindings() )
  {
    if ( binding.name == "size" )
      sizeBinding = &binding;
    else if ( binding.name == "min_uv" )
      minUvBinding = &binding;
    else if ( binding.name == "max_uv" )
      maxUvBinding = &binding;
  }

  ASSERT_NE( nullptr, sizeBinding );
  ASSERT_NE( nullptr, minUvBinding );
  ASSERT_NE( nullptr, maxUvBinding );

  Vector2 size, minUv, maxUv;
  ASSERT_EQ( sizeof( Vector2 ), sizeBinding->payload.size() );
  std::memcpy( &size, sizeBinding->payload.data(), sizeof( Vector2 ) );
  ASSERT_EQ( sizeof( Vector2 ), minUvBinding->payload.size() );
  std::memcpy( &minUv, minUvBinding->payload.data(), sizeof( Vector2 ) );
  ASSERT_EQ( sizeof( Vector2 ), maxUvBinding->payload.size() );
  std::memcpy( &maxUv, maxUvBinding->payload.data(), sizeof( Vector2 ) );

  EXPECT_FLOAT_EQ( 48.0f, size.x );
  EXPECT_FLOAT_EQ( 48.0f, size.y );
  EXPECT_FLOAT_EQ( 0.25f, minUv.x );
  EXPECT_FLOAT_EQ( 0.0f, minUv.y );
  EXPECT_FLOAT_EQ( 1.0f, maxUv.x );
  EXPECT_FLOAT_EQ( 0.75f, maxUv.y );

  EXPECT_EQ( std::size_t( 4 ), binder.GetBindings().size() );

  const UniformBinding* offsetBinding = nullptr;
  for ( const UniformBinding& binding : binder.GetBindings() )
  {
    if ( binding.name == "offset" )
    {
      offsetBinding = &binding;
      break;
    }
  }

  ASSERT_NE( nullptr, offsetBinding );
  ASSERT_EQ( sizeof( Vector2 ), offsetBinding->payload.size() );

  Vector2 offset;
  std::memcpy( &offset, offsetBinding->payload.data(), sizeof( Vector2 ) );
  EXPECT_FLOAT_EQ( 0.0f, offset.x );
  EXPECT_FLOAT_EQ( 0.0f, offset.y );

  const auto& samplers = binder.GetSamplerBindings();
  ASSERT_EQ( std::size_t( 1 ), samplers.size() );
  ASSERT_EQ( "diffuseTexture", samplers.front().name );
  ASSERT_EQ( 0, samplers.front().unit );
  ASSERT_EQ( texture, samplers.front().tex );

  struct FlipCase
  {
    int flags;
    float minU;
    float minV;
    float maxU;
    float maxV;
  };

  const FlipCase cases[] = { { SAF_FLIPX, 1.0f, 0.0f, 0.25f, 0.75f },
                             { SAF_FLIPY, 0.25f, 0.75f, 1.0f, 0.0f },
                             { SAF_FLIPX | SAF_FLIPY, 1.0f, 0.75f, 0.25f, 0.0f },
                             { SAF_NONE, 0.25f, 0.0f, 1.0f, 0.75f } };

  for ( const FlipCase& testCase : cases )
  {
    SCOPED_TRACE( testCase.flags );

    SpriteAtlasUtils::SetSpriteFrame( device, &material, &object, &atlas, "frame_b", testCase.flags );

    UniformBinder flippedBinder;
    firstProvider( flippedBinder );

    const UniformBinding* flippedMin = nullptr;
    const UniformBinding* flippedMax = nullptr;
    for ( const UniformBinding& binding : flippedBinder.GetBindings() )
    {
      if ( binding.name == "min_uv" )
        flippedMin = &binding;
      else if ( binding.name == "max_uv" )
        flippedMax = &binding;
    }

    ASSERT_NE( nullptr, flippedMin );
    ASSERT_NE( nullptr, flippedMax );
    ASSERT_EQ( sizeof( Vector2 ), flippedMin->payload.size() );
    ASSERT_EQ( sizeof( Vector2 ), flippedMax->payload.size() );

    Vector2 actualMin, actualMax;
    std::memcpy( &actualMin, flippedMin->payload.data(), sizeof( Vector2 ) );
    std::memcpy( &actualMax, flippedMax->payload.data(), sizeof( Vector2 ) );

    EXPECT_FLOAT_EQ( testCase.minU, actualMin.x );
    EXPECT_FLOAT_EQ( testCase.minV, actualMin.y );
    EXPECT_FLOAT_EQ( testCase.maxU, actualMax.x );
    EXPECT_FLOAT_EQ( testCase.maxV, actualMax.y );

    ASSERT_EQ( std::size_t( 1 ), object.GetSlot( 0 ).providers.size() );
    EXPECT_EQ( firstGeometry, object.GetSlot( 0 ).geometry );
    EXPECT_EQ( firstInputLayout, object.GetSlot( 0 ).inputLayout );
  }
}
