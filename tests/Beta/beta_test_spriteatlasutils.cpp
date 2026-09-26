#include <Nebulae/Alpha/Shaders/UniformBinder.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Common/FileSystem/File.h>
#include <Mock/MockRenderSystem.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cstring>

using namespace Nebulae;

namespace
{
class StringFile : public File
{
public:
  explicit StringFile( std::string contents ) : m_contents( std::move( contents ) ) {}

  size_t Read( void* buffer, size_t size ) override
  {
    const size_t count = std::min( size, m_contents.size() - m_position );
    std::memcpy( buffer, m_contents.data() + m_position, count );
    m_position += count;
    return count;
  }
  void Seek( size_t position ) override { m_position = position; }
  void SeekToEnd() override { m_position = m_contents.size(); }
  size_t Tell() const override { return m_position; }

private:
  std::string m_contents;
  size_t m_position = 0;
};

class TestTextureImpl : public TextureImpl
{
public:
  explicit TestTextureImpl( const std::string& name ) : TextureImpl( name )
  {
    m_width = 16;
    m_height = 16;
  }
};

Vector2 GetVector( const UniformBinder& binder, const char* name )
{
  for ( const auto& binding : binder.GetBindings() )
  {
    if ( binding.name == name && binding.payload.size() == sizeof( Vector2 ) )
    {
      Vector2 value;
      std::memcpy( &value, binding.payload.data(), sizeof( value ) );
      return value;
    }
  }
  return Vector2( -1.0f, -1.0f );
}
} // namespace

TEST( SpriteAtlasUtils, RepeatedFrameChangesKeepProviderAndGeometryAndReadLatestState )
{
  auto files = std::make_shared<FileSystem>();
  auto renderer = std::make_shared<testing::NiceMock<MockRenderDevice>>( files, nullptr );
  ASSERT_TRUE( renderer->Initiate() );
  ON_CALL( *renderer, CreateTextureImpl( testing::_ ) )
    .WillByDefault( []( const std::string& name ) { return new TestTextureImpl( name ); } );
  Texture* texture = renderer->CreateTexture( "sheet.png", false );
  ASSERT_NE( nullptr, texture );

  StringFile atlasFile( R"({"meta":{"image":"sheet.png","size":{"w":16,"h":16}},"frames":[{"filename":"first","frame":{"x":0,"y":0,"w":4,"h":6}},{"filename":"second","frame":{"x":4,"y":4,"w":8,"h":3}}]})" );
  SpriteAtlas atlas( "atlas", renderer );
  ASSERT_TRUE( atlas.Load( &atlasFile ) );
  SubTexture* second = atlas.FindModuleSubTexture( "second" );
  ASSERT_NE( nullptr, second );

  Material material( "sprite" );
  material.CreatePass();
  Material other( "unrelated" );
  SceneObject object( nullptr );
  object.AddSlot( &other ); // The sprite material is not yet attached.
  SpriteAtlasUtils::SetSpriteFrame( renderer, &material, &object, &atlas, "first" );
  ASSERT_EQ( 2u, object.GetSlotCount() );
  EXPECT_TRUE( object.GetSlot( 0 ).providers.empty() );
  EXPECT_EQ( nullptr, object.GetSlot( 0 ).geometry );
  EXPECT_EQ( &material, object.GetSlot( 1 ).material );
  ASSERT_EQ( 1u, object.GetSlot( 1 ).providers.size() );
  auto firstProvider = object.GetSlot( 1 ).providers[0].second;
  Geometry* geometry = object.GetSlot( 1 ).geometry;
  ASSERT_NE( nullptr, geometry );

  SpriteAtlasUtils::SetSpriteFrame( renderer, &material, &object, &atlas, "second", SAF_FLIPX );
  ASSERT_EQ( 2u, object.GetSlotCount() );
  EXPECT_TRUE( object.GetSlot( 0 ).providers.empty() );
  ASSERT_EQ( 1u, object.GetSlot( 1 ).providers.size() );
  EXPECT_EQ( geometry, object.GetSlot( 1 ).geometry );

  UniformBinder binder;
  firstProvider( binder ); // The original callback must see the latest frame.
  ASSERT_EQ( 4u, binder.GetBindings().size() );
  ASSERT_EQ( 1u, binder.GetSamplerBindings().size() );
  EXPECT_FLOAT_EQ( 8.0f, GetVector( binder, "size" ).x );
  EXPECT_FLOAT_EQ( 3.0f, GetVector( binder, "size" ).y );
  EXPECT_FLOAT_EQ( 0.0f, GetVector( binder, "offset" ).x );
  EXPECT_FLOAT_EQ( 0.0f, GetVector( binder, "offset" ).y );
  EXPECT_FLOAT_EQ( second->GetTexCoords()[2], GetVector( binder, "min_uv" ).x );
  EXPECT_FLOAT_EQ( second->GetTexCoords()[1], GetVector( binder, "min_uv" ).y );
  EXPECT_FLOAT_EQ( second->GetTexCoords()[0], GetVector( binder, "max_uv" ).x );
  EXPECT_FLOAT_EQ( second->GetTexCoords()[3], GetVector( binder, "max_uv" ).y );
  EXPECT_EQ( texture, binder.GetSamplerBindings()[0].tex );
}
