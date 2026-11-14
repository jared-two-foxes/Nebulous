#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/FileSystem.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/StreamCompiler.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationController.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>

#include <Mock/MockRenderSystem.h>
#include <Mock/MockWindow.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstring>
#include <functional>
#include <memory>

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

bool ReadVec2( const UniformBinder& binder, const char* name, Vector2& out )
{
  for ( const UniformBinding& binding : binder.GetBindings() )
  {
    if ( binding.name == name && binding.payload.size() == sizeof( Vector2 ) )
    {
      std::memcpy( &out, binding.payload.data(), sizeof( Vector2 ) );
      return true;
    }
  }
  return false;
}

struct CapturedSprite
{
  Vector2 size;
  Vector2 minUv;
  Vector2 maxUv;
  const Texture* texture = nullptr;
  int draws = 0;
};

// Reads the sprite uniforms/sampler the StreamCompiler wrote; locations match the schema in the test.
CapturedSprite CaptureSprite( const RenderStream& stream )
{
  CapturedSprite captured;
  const auto* base = stream.Data();
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, base + offset, sizeof( header ) );
    if ( header.size == 0 )
    {
      break;
    }

    if ( header.type == PT_Draw )
    {
      ++captured.draws;
    }
    else if ( header.type == PT_SetUniform )
    {
      PacketSetUniform packet{};
      std::memcpy( &packet, base + offset, sizeof( packet ) );
      if ( packet.write.payloadBytes == sizeof( Vector2 ) )
      {
        Vector2 value;
        std::memcpy( &value, base + offset + sizeof( PacketSetUniform ), sizeof( Vector2 ) );
        switch ( packet.write.gpuLocation )
        {
        case 0:
          captured.size = value;
          break;
        case 2:
          captured.minUv = value;
          break;
        case 3:
          captured.maxUv = value;
          break;
        default:
          break;
        }
      }
    }
    else if ( header.type == PT_SetSampler )
    {
      PacketSetSampler packet{};
      std::memcpy( &packet, base + offset, sizeof( packet ) );
      if ( packet.write.gpuLocation == 4 )
      {
        captured.texture = packet.write.tex;
      }
    }

    offset += header.size;
  }
  return captured;
}

class SpriteAnimationControllerFixture : public ::testing::Test
{
protected:
  std::shared_ptr<FileSystem> fileSystem;
  std::shared_ptr<Window> window;
  std::shared_ptr<MockRenderDevice> device;
  Texture* texture = nullptr;
  std::unique_ptr<SpriteAtlas> atlas;
  std::unique_ptr<HardwareShader> vertexShader;
  Material material{ "sprite_animation_material" };
  Pass* pass = nullptr;
  SceneNode node{ nullptr };
  std::unique_ptr<SpriteAnimation> animation;

  void SetUp() override
  {
    fileSystem = std::shared_ptr<FileSystem>( new FileSystem() );
    fileSystem->Mount( "disk", new DiskFileDevice( "../../tests/Assets" ) );
    window = std::shared_ptr<MockWindow>( new MockWindow() );
    device = std::shared_ptr<MockRenderDevice>( new NiceMock<MockRenderDevice>( fileSystem, window ) );
    ASSERT_TRUE( device->Initiate() );

    texture = device->CreateTexture( "atlas_texture", false );
    ASSERT_NE( nullptr, texture );
    texture->SetImpl( new TestTextureImpl( "atlas_texture", 64, 64 ) );

    atlas.reset( new SpriteAtlas( "test_atlas", device ) );
    std::unique_ptr<File> stream( fileSystem->Open( "disk", "spriteatlas_test.json" ) );
    ASSERT_NE( nullptr, stream.get() );
    ASSERT_TRUE( atlas->Load( stream.get() ) );
    ASSERT_NE( nullptr, atlas->FindModuleSubTexture( "frame_a" ) );
    ASSERT_NE( nullptr, atlas->FindModuleSubTexture( "frame_b" ) );

    ON_CALL( *device, CreateInputLayoutImpl( testing::_, testing::_ ) )
      .WillByDefault( []( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader ) -> InputLayoutImpl*
                      { return new TestInputLayoutImpl( pVertexDecl, pVertexShader ); } );
    vertexShader.reset( new HardwareShader( "sprite_animation_vs", device.get() ) );
    pass = material.CreatePass();
    pass->SetVertexShader( vertexShader.get() );

    animation.reset( new SpriteAnimation( "test_animation", nullptr ) );
    animation->AddFrame( atlas.get(), "frame_a" );
    animation->AddFrame( atlas.get(), "frame_b" );
  }

  SceneObject* FindObject() const { return node.FindSubObject( &material ); }

  void ExpectFrame( const SpriteFrameState& state, const char* frameName, int flags = 0 ) const
  {
    SCOPED_TRACE( frameName );
    SubTexture* sub = atlas->FindModuleSubTexture( frameName );
    ASSERT_NE( nullptr, sub );
    const auto* tc = sub->GetTexCoords();
    const bool flipX = ( flags & SAF_FLIPX ) != 0;
    const bool flipY = ( flags & SAF_FLIPY ) != 0;

    EXPECT_FLOAT_EQ( static_cast<float>( sub->GetWidth() ), state.size.x );
    EXPECT_FLOAT_EQ( static_cast<float>( sub->GetHeight() ), state.size.y );
    EXPECT_FLOAT_EQ( flipX ? tc[2] : tc[0], state.minUv.x );
    EXPECT_FLOAT_EQ( flipY ? tc[3] : tc[1], state.minUv.y );
    EXPECT_FLOAT_EQ( flipX ? tc[0] : tc[2], state.maxUv.x );
    EXPECT_FLOAT_EQ( flipY ? tc[1] : tc[3], state.maxUv.y );
    EXPECT_EQ( sub->GetTexture(), state.diffuseTexture );
  }
};
} // namespace

TEST_F( SpriteAnimationControllerFixture, Play_CreatesOneObjectWithOneSpriteProvider )
{
  SpriteAnimationController controller( device, &material, &node );
  controller.Play( animation.get() );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );
  ASSERT_EQ( std::size_t( 1 ), object->GetSlotCount() );
  ASSERT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
  EXPECT_EQ( "sprite_frame", object->GetSlot( 0 ).providers.front().first );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a" );
}

TEST_F( SpriteAnimationControllerFixture, Update_WritesPersistentStateWithoutRegisteringProviders )
{
  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( animation.get() );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );
  const SpriteFrameState* state = &object->GetSpriteFrameState();
  const Geometry* geometry = object->GetSlot( 0 ).geometry;
  const InputLayout* inputLayout = object->GetSlot( 0 ).inputLayout;
  ASSERT_NE( nullptr, geometry );
  ASSERT_NE( nullptr, inputLayout );

  // Instrument the registered provider: if Update replaced it the wrapper would be lost.
  UniformProvider firstProvider = object->GetSlot( 0 ).providers.front().second;
  int invocations = 0;
  object->AddProvider( "sprite_frame",
                       [firstProvider, &invocations]( UniformBinder& binder )
                       {
                         ++invocations;
                         firstProvider( binder );
                       } );

  controller.Update( 1.5f );
  ExpectFrame( *state, "frame_b" );
  EXPECT_EQ( object, FindObject() );
  EXPECT_EQ( state, &object->GetSpriteFrameState() );
  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
  EXPECT_EQ( geometry, object->GetSlot( 0 ).geometry );
  EXPECT_EQ( inputLayout, object->GetSlot( 0 ).inputLayout );

  controller.Update( 1.5f ); // wraps back to the first frame
  ExpectFrame( *state, "frame_a" );
  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
  EXPECT_EQ( geometry, object->GetSlot( 0 ).geometry );
  EXPECT_EQ( inputLayout, object->GetSlot( 0 ).inputLayout );

  UniformBinder binder;
  object->GetSlot( 0 ).providers.front().second( binder );
  EXPECT_EQ( 1, invocations ) << "Update must not replace the registered sprite provider.";

  Vector2 size;
  ASSERT_TRUE( ReadVec2( binder, "size", size ) );
  EXPECT_FLOAT_EQ( 32.0f, size.x );
  EXPECT_FLOAT_EQ( 32.0f, size.y );
}

TEST_F( SpriteAnimationControllerFixture, Update_InvokesCompleteCallbackWhenLooping )
{
  int completions = 0;
  std::function<void()> callback = [&completions]() { ++completions; };

  SpriteAnimationController controller( device, &material, &node );
  controller.SetCompleteCallback( callback, nullptr );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( animation.get() );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );

  controller.Update( 0.5f ); // not enough time to advance
  EXPECT_EQ( 0, completions );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a" );

  controller.Update( 1.0f ); // advances to the second frame
  EXPECT_EQ( 0, completions );
  ExpectFrame( object->GetSpriteFrameState(), "frame_b" );

  controller.Update( 1.0f ); // wraps: callback fires, first frame is applied
  EXPECT_EQ( 1, completions );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a" );
}

TEST_F( SpriteAnimationControllerFixture, Update_HonoursDisplayFlags )
{
  const int flags = SAF_FLIPX | SAF_FLIPY;
  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( animation.get(), flags );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a", flags );

  controller.Update( 1.5f );
  ExpectFrame( object->GetSpriteFrameState(), "frame_b", flags );
}

TEST_F( SpriteAnimationControllerFixture, Stop_ClearsTheNode )
{
  SpriteAnimationController controller( device, &material, &node );
  controller.Play( animation.get() );
  ASSERT_NE( nullptr, FindObject() );

  controller.Stop();
  EXPECT_EQ( nullptr, FindObject() );
  EXPECT_EQ( nullptr, controller.GetCurrentFrame() );

  controller.Update( 10.0f ); // no object and no animation: must be a no-op
  EXPECT_EQ( nullptr, FindObject() );
}

TEST_F( SpriteAnimationControllerFixture, CompiledStream_ReflectsTheCurrentFrame )
{
  UniformDefinitionMap schema;
  auto define = [&schema]( const char* name, UniformType type, std::size_t index )
  {
    UniformDefinitionBase definition;
    definition.type = type;
    definition.logicalIndex = index;
    schema[name] = definition;
  };
  define( "size", UT_FLOAT2, 0 );
  define( "offset", UT_FLOAT2, 1 );
  define( "min_uv", UT_FLOAT2, 2 );
  define( "max_uv", UT_FLOAT2, 3 );
  define( "diffuseTexture", UT_SAMPLER2D, 4 );
  pass->SetUniformSchema( schema );

  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( animation.get() );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );

  // The mock renderer may not hand back a real vertex buffer; the compiler skips slots without one.
  HardwareBuffer standInBuffer( "sprite_animation_vb", nullptr );
  ASSERT_NE( nullptr, object->GetSlot( 0 ).geometry );
  if ( object->GetSlot( 0 ).geometry->m_vertexBuffer == nullptr )
  {
    object->GetSlot( 0 ).geometry->m_vertexBuffer = &standInBuffer;
  }

  auto compile = [&]( RenderStream& stream )
  {
    DrawItemList items;
    object->EmitDrawItems( items, 0, 0 );
    StreamCompiler compiler;
    compiler.Compile( items, nullptr, stream );
  };

  auto expectCaptured = [&]( const RenderStream& stream, const char* frameName )
  {
    SCOPED_TRACE( frameName );
    SubTexture* sub = atlas->FindModuleSubTexture( frameName );
    ASSERT_NE( nullptr, sub );
    const auto* tc = sub->GetTexCoords();
    const CapturedSprite captured = CaptureSprite( stream );
    EXPECT_EQ( 1, captured.draws );
    EXPECT_FLOAT_EQ( static_cast<float>( sub->GetWidth() ), captured.size.x );
    EXPECT_FLOAT_EQ( static_cast<float>( sub->GetHeight() ), captured.size.y );
    EXPECT_FLOAT_EQ( tc[0], captured.minUv.x );
    EXPECT_FLOAT_EQ( tc[1], captured.minUv.y );
    EXPECT_FLOAT_EQ( tc[2], captured.maxUv.x );
    EXPECT_FLOAT_EQ( tc[3], captured.maxUv.y );
    EXPECT_EQ( sub->GetTexture(), captured.texture );
  };

  RenderStream stream;
  compile( stream );
  expectCaptured( stream, "frame_a" );

  controller.Update( 1.5f );
  compile( stream );
  expectCaptured( stream, "frame_b" );
  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
}

TEST_F( SpriteAnimationControllerFixture, Play_MissingFirstFrame_BecomesRenderableOnLaterFrames )
{
  SpriteAnimation missingFirst( "missing_first_frame", nullptr );
  missingFirst.AddFrame( atlas.get(), "no_such_frame" );
  missingFirst.AddFrame( atlas.get(), "frame_b" );

  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( &missingFirst );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );
  ASSERT_EQ( std::size_t( 1 ), object->GetSlotCount() );
  ASSERT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
  EXPECT_EQ( "sprite_frame", object->GetSlot( 0 ).providers.front().first );
  EXPECT_NE( nullptr, object->GetSlot( 0 ).geometry );
  EXPECT_NE( nullptr, object->GetSlot( 0 ).inputLayout );

  controller.Update( 1.5f );
  ExpectFrame( object->GetSpriteFrameState(), "frame_b" );
  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
}

TEST_F( SpriteAnimationControllerFixture, Update_NullAtlasFrame_KeepsPreviousAppearance )
{
  SpriteAnimation nullAtlas( "null_atlas_frame", nullptr );
  nullAtlas.AddFrame( atlas.get(), "frame_a" );
  nullAtlas.AddFrame( nullptr, "frame_b" );

  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( &nullAtlas );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a" );

  controller.Update( 1.5f );
  ExpectFrame( object->GetSpriteFrameState(), "frame_a" );
  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
}

TEST_F( SpriteAnimationControllerFixture, Update_AcrossAtlasTextures_SwitchesTextureWithoutNewProviders )
{
  Texture* textureB = device->CreateTexture( "atlas_texture_b", false );
  ASSERT_NE( nullptr, textureB );
  textureB->SetImpl( new TestTextureImpl( "atlas_texture_b", 64, 64 ) );

  SpriteAtlas atlasB( "test_atlas_b", device );
  std::unique_ptr<File> streamB( fileSystem->Open( "disk", "spriteatlas_test_b.json" ) );
  ASSERT_NE( nullptr, streamB.get() );
  ASSERT_TRUE( atlasB.Load( streamB.get() ) );
  SubTexture* frameC = atlasB.FindModuleSubTexture( "frame_c" );
  ASSERT_NE( nullptr, frameC );
  ASSERT_NE( texture, textureB );
  ASSERT_EQ( texture, atlas->FindModuleSubTexture( "frame_a" )->GetTexture() );
  ASSERT_EQ( textureB, frameC->GetTexture() );

  SpriteAnimation crossAtlas( "cross_atlas", nullptr );
  crossAtlas.AddFrame( atlas.get(), "frame_a" );
  crossAtlas.AddFrame( &atlasB, "frame_c" );

  UniformDefinitionMap schema;
  auto define = [&schema]( const char* name, UniformType type, std::size_t index )
  {
    UniformDefinitionBase definition;
    definition.type = type;
    definition.logicalIndex = index;
    schema[name] = definition;
  };
  define( "size", UT_FLOAT2, 0 );
  define( "offset", UT_FLOAT2, 1 );
  define( "min_uv", UT_FLOAT2, 2 );
  define( "max_uv", UT_FLOAT2, 3 );
  define( "diffuseTexture", UT_SAMPLER2D, 4 );
  pass->SetUniformSchema( schema );

  SpriteAnimationController controller( device, &material, &node );
  controller.SetPlaybackSpeed( 1.0f );
  controller.Play( &crossAtlas );

  SceneObject* object = FindObject();
  ASSERT_NE( nullptr, object );

  HardwareBuffer standInBuffer( "sprite_animation_vb", nullptr );
  ASSERT_NE( nullptr, object->GetSlot( 0 ).geometry );
  if ( object->GetSlot( 0 ).geometry->m_vertexBuffer == nullptr )
  {
    object->GetSlot( 0 ).geometry->m_vertexBuffer = &standInBuffer;
  }

  auto compile = [&]( RenderStream& stream )
  {
    DrawItemList items;
    object->EmitDrawItems( items, 0, 0 );
    StreamCompiler compiler;
    compiler.Compile( items, nullptr, stream );
  };

  const Geometry* geometry = object->GetSlot( 0 ).geometry;
  const InputLayout* inputLayout = object->GetSlot( 0 ).inputLayout;

  EXPECT_EQ( texture, object->GetSpriteFrameState().diffuseTexture );
  RenderStream firstStream;
  compile( firstStream );
  EXPECT_EQ( texture, CaptureSprite( firstStream ).texture );

  controller.Update( 1.5f );
  EXPECT_EQ( textureB, object->GetSpriteFrameState().diffuseTexture );
  EXPECT_FLOAT_EQ( 16.0f, object->GetSpriteFrameState().size.x );
  EXPECT_FLOAT_EQ( 16.0f, object->GetSpriteFrameState().size.y );

  RenderStream secondStream;
  compile( secondStream );
  const CapturedSprite captured = CaptureSprite( secondStream );
  EXPECT_EQ( 1, captured.draws );
  EXPECT_EQ( textureB, captured.texture );
  EXPECT_FLOAT_EQ( 16.0f, captured.size.x );

  EXPECT_EQ( std::size_t( 1 ), object->GetSlot( 0 ).providers.size() );
  EXPECT_EQ( geometry, object->GetSlot( 0 ).geometry );
  EXPECT_EQ( inputLayout, object->GetSlot( 0 ).inputLayout );
}
