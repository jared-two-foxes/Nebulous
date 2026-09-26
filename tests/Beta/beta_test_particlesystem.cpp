#include <Nebulae/Common/Common.h>

// Inspect ownership after Clear and DestroyEmitter; production access remains private.
#define private public
#include <Nebulae/Beta/Particle/ParticleSystem.h>
#undef private

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Particle/ParticleEmitter.h>
#include <Nebulae/Beta/Particle/ParticleGroup.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>
#include <Nebulae/Common/FileSystem/FileDevice.h>
#include <Mock/MockRenderSystem.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cstring>
#include <map>

using namespace Nebulae;

namespace
{
class TestTextureImpl : public TextureImpl
{
public:
  explicit TestTextureImpl( const std::string& name ) : TextureImpl( name )
  {
    m_width = 8;
    m_height = 8;
  }
};

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

class StringFileDevice : public FileDevice
{
public:
  std::map<std::string, std::string> files;

  File* Open( const std::string& path, FileSystem::Mode ) override
  {
    const auto it = files.find( path );
    return it == files.end() ? nullptr : new StringFile( it->second );
  }
  File* Open( File* ) override { return nullptr; }
  void Close( File* file ) override { delete file; }
};

class ParticleSystemTest : public ::testing::Test
{
protected:
  StringFileDevice files;
  std::shared_ptr<FileSystem> fileSystem = std::make_shared<FileSystem>();
  std::shared_ptr<testing::NiceMock<MockRenderDevice>> renderer;

  void SetUp() override
  {
    files.files["group.json"] = R"({"texture": "particle.png", "life": 1, "scale": 1})";
    files.files["emitter.json"] = R"({"tank": [{"group": "group.json", "count": 2, "flow": 0}], "force": {"x": 0, "y": 0, "z": 0, "w": 0}})";
    files.files["default_particle_vs.glsl"] = "void main() {}";
    files.files["default_particle_fs.glsl"] = "void main() {}";
    fileSystem->Mount( NE_DEFAULT_ROOTDEVICE, &files );
    renderer = std::make_shared<testing::NiceMock<MockRenderDevice>>( fileSystem, nullptr );
    ASSERT_TRUE( renderer->Initiate() );
    ON_CALL( *renderer, CreateTextureImpl( testing::_ ) )
      .WillByDefault( []( const std::string& name ) { return new TestTextureImpl( name ); } );
    ON_CALL( *renderer, CreateInputLayoutImpl( testing::_, testing::_ ) )
      .WillByDefault( []( VertexDeceleration* decl, HardwareShader* shader )
                      { return new InputLayoutImpl( decl, shader ); } );
  }
};
} // namespace

TEST_F( ParticleSystemTest, LoadsGroupsAndEmittersThenAdvancesAndExpiresParticles )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ParticleGroup* group = system.CreateGroup( "group.json" );
  ASSERT_NE( nullptr, group );
  EXPECT_EQ( group, system.CreateGroup( "group.json" ) );

  ParticleEmitter* emitter = system.CreateEmitter( "emitter.json" );
  ASSERT_NE( nullptr, emitter );
  ASSERT_EQ( 1u, system.m_groups.size() );
  ASSERT_EQ( 1u, system.m_emitters.size() );
  emitter->Start();

  system.Update( 0 );
  ASSERT_EQ( 2u, group->GetParticleCount() );
  Camera camera;
  system.SetCamera( &camera );
  EXPECT_EQ( &camera, system.GetCamera() );
  EXPECT_CALL( *renderer, Draw( 6, 0 ) ).Times( 2 );
  system.Render(); // Draws both live particles using the group's texture.

  system.Update( 2000000 );
  EXPECT_EQ( 0u, group->GetParticleCount() );
  EXPECT_NO_THROW( system.Render() ); // No live particles remain.
}

TEST_F( ParticleSystemTest, RejectsGroupsWithoutAnImage )
{
  files.files["missing-image.json"] = R"({"life": 1, "scale": 1})";
  ParticleSystem system( fileSystem, renderer, nullptr );
  EXPECT_EQ( nullptr, system.CreateGroup( "missing-image.json" ) );
  EXPECT_TRUE( system.m_groups.empty() );
}

TEST_F( ParticleSystemTest, ClearingOneGroupKeepsSharedAtlasFrameUsable )
{
  files.files["atlas.json"] = R"({"meta":{"image":"particle.png","size":{"w":8,"h":8}},"frames":[{"filename":"spark","frame":{"x":0,"y":0,"w":8,"h":8}}]})";
  files.files["atlas-group.json"] = R"({"atlas":"atlas.json","frame":"spark","life":1})";
  auto atlases = std::make_shared<SpriteAtlasManager>( fileSystem, renderer );
  ParticleSystem first( fileSystem, renderer, atlases );
  ParticleSystem second( fileSystem, renderer, atlases );
  ASSERT_NE( nullptr, first.CreateGroup( "atlas-group.json" ) );
  ParticleGroup* remaining = second.CreateGroup( "atlas-group.json" );
  ASSERT_NE( nullptr, remaining );
  SubTexture* frame = atlases->GetByName( "atlas.json" )->FindModuleSubTexture( "spark" );
  ASSERT_NE( nullptr, frame );

  first.Clear();
  EXPECT_EQ( frame, atlases->GetByName( "atlas.json" )->FindModuleSubTexture( "spark" ) );
  EXPECT_EQ( 8, frame->GetWidth() );
  ASSERT_NE( nullptr, remaining->SpawnParticle() );
  Camera camera;
  second.SetCamera( &camera );
  EXPECT_CALL( *renderer, Draw( 6, 0 ) ).Times( 1 );
  second.Render();
}

TEST_F( ParticleSystemTest, DestroyEmitterAndClearReleaseOwnedObjects )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ParticleEmitter* emitter = system.CreateEmitter( "emitter.json" );
  ASSERT_NE( nullptr, emitter );
  ASSERT_EQ( 1u, system.m_groups.size() );
  ASSERT_EQ( 1u, system.m_emitters.size() );

  system.DestroyEmitter( emitter );
  EXPECT_TRUE( system.m_emitters.empty() );
  EXPECT_EQ( 1u, system.m_groups.size() );

  system.CreateEmitter( "emitter.json" );
  system.Clear();
  EXPECT_TRUE( system.m_emitters.empty() );
  EXPECT_TRUE( system.m_groups.empty() );
  system.Update( 1000000 );
  EXPECT_NO_THROW( system.Render() );
}
