#include <Nebulae/Common/Common.h>

// Inspect ownership after Clear and DestroyEmitter; production access remains private.
#define private public
#include <Nebulae/Beta/Particle/ParticleSystem.h>
#undef private

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/Particle/Particle.h>
#include <Nebulae/Beta/Particle/ParticleEmitter.h>
#define private public
#include <Nebulae/Beta/Particle/ParticleGroup.h>
#undef private
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/StreamCompiler.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
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

UniformDefinitionMap ParticleSchema()
{
  UniformDefinitionMap schema;
  std::size_t index = 0;
  const auto add = [&]( const char* name, UniformType type )
  {
    UniformDefinitionBase def;
    def.type = type;
    def.logicalIndex = index++;
    schema[name] = def;
  };
  add( "view", UT_MATRIX_4X4 );
  add( "projection", UT_MATRIX_4X4 );
  add( "world", UT_MATRIX_4X4 );
  add( "size", UT_FLOAT2 );
  add( "offset", UT_FLOAT2 );
  add( "min_uv", UT_FLOAT2 );
  add( "max_uv", UT_FLOAT2 );
  add( "diffuseTexture", UT_SAMPLER2D );
  return schema;
}

std::size_t CountPackets( const RenderStream& stream, PacketType type )
{
  std::size_t count = 0;
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    if ( header.size == 0 )
      break;
    count += header.type == type;
    offset += header.size;
  }
  return count;
}

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
    // The particle pass needs its shader pair reflected, otherwise StreamCompiler drops every uniform.
    ON_CALL( *renderer, ReflectProgramShaders( testing::_, testing::_, testing::_ ) )
      .WillByDefault(
        []( HardwareShader*, HardwareShader*, UniformDefinitionMap& out )
        {
          out = ParticleSchema();
          return true;
        } );
    ON_CALL( *renderer, CreateTextureImpl( testing::_ ) )
      .WillByDefault( []( const std::string& name ) { return new TestTextureImpl( name ); } );
  }
};
} // namespace

TEST_F( ParticleSystemTest, LoadsGroupsAndEmittersThenAdvancesAndExpiresParticles )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ParticleGroup* group = system.CreateGroup( "group.json" );
  ASSERT_NE( nullptr, group );
  EXPECT_EQ( group->m_ownedTexture.get(), group->m_pTexture );
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
  DrawItemList items;
  system.EmitDrawItems( items );
  EXPECT_EQ( 2u, items.Size() ); // Both live particles emit a DrawItem.

  system.Update( 2000000 );
  EXPECT_EQ( 0u, group->GetParticleCount() );
  DrawItemList emptyItems;
  EXPECT_NO_THROW( system.EmitDrawItems( emptyItems ) ); // No live particles remain.
}

TEST_F( ParticleSystemTest, RejectsGroupsWithoutAnImage )
{
  files.files["missing-image.json"] = R"({"life": 1, "scale": 1})";
  ParticleSystem system( fileSystem, renderer, nullptr );
  EXPECT_EQ( nullptr, system.CreateGroup( "missing-image.json" ) );
  EXPECT_TRUE( system.m_groups.empty() );
}

TEST_F( ParticleSystemTest, ClearingAtlasGroupKeepsAtlasFrameUsable )
{
  files.files["atlas.json"] = R"({"meta":{"image":"particle.png","size":{"w":8,"h":8}},"frames":[{"filename":"spark","frame":{"x":0,"y":0,"w":8,"h":8}}]})";
  files.files["atlas-group.json"] = R"({"atlas":"atlas.json","frame":"spark","life":1})";
  auto atlases = std::make_shared<SpriteAtlasManager>( fileSystem, renderer );
  ParticleSystem system( fileSystem, renderer, atlases );
  ParticleGroup* group = system.CreateGroup( "atlas-group.json" );
  ASSERT_NE( nullptr, group );
  SubTexture* frame = atlases->GetByName( "atlas.json" )->FindModuleSubTexture( "spark" );
  ASSERT_NE( nullptr, frame );
  EXPECT_EQ( nullptr, group->m_ownedTexture.get() );
  EXPECT_EQ( frame, group->m_pTexture );

  ASSERT_NE( nullptr, group->SpawnParticle() );
  Camera camera;
  system.SetCamera( &camera );
  DrawItemList items;
  system.EmitDrawItems( items );
  EXPECT_EQ( 1u, items.Size() ); // One live particle emits a DrawItem.

  system.Clear();
  EXPECT_EQ( frame, atlases->GetByName( "atlas.json" )->FindModuleSubTexture( "spark" ) );
  EXPECT_EQ( 8, frame->GetWidth() );
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
  DrawItemList items;
  EXPECT_NO_THROW( system.EmitDrawItems( items ) );
}

TEST_F( ParticleSystemTest, LoadLinksShadersBeforeReflectingTheirSchema )
{
  testing::InSequence order;
  EXPECT_CALL( *renderer, SetShaders( testing::_, testing::_ ) ).Times( 1 );
  EXPECT_CALL( *renderer, ReflectProgramShaders( testing::_, testing::_, testing::_ ) ).Times( 1 );
  ParticleSystem system( fileSystem, renderer, nullptr );
  ParticleGroup* group = system.CreateGroup( "group.json" );
  ASSERT_NE( nullptr, group );
  EXPECT_EQ( ParticleSchema().size(), group->m_pMaterial->GetPass( 0 )->GetUniformSchema().size() );
}

TEST_F( ParticleSystemTest, RejectsGroupsWhoseShadersCannotBeReflected )
{
  ON_CALL( *renderer, ReflectProgramShaders( testing::_, testing::_, testing::_ ) )
    .WillByDefault( testing::Return( false ) );
  ParticleSystem system( fileSystem, renderer, nullptr );
  EXPECT_EQ( nullptr, system.CreateGroup( "group.json" ) );
  EXPECT_TRUE( system.m_groups.empty() );
}

TEST_F( ParticleSystemTest, EmittedParticlesCompileToDrawsWithBindingsAndTransparentState )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ASSERT_NE( nullptr, system.CreateEmitter( "emitter.json" ) );
  system.m_emitters.front()->Start();
  system.Update( 0 );
  Camera camera;
  system.SetCamera( &camera );

  DrawItemList items;
  system.EmitDrawItems( items );
  ASSERT_EQ( 2u, items.Size() );

  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, &camera, stream );

  // One complete draw per particle.
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetProgram ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetGeometry ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_Draw ) );
  // view, projection, world, size, offset, min_uv, max_uv per draw, plus the diffuse sampler.
  EXPECT_EQ( 14u, CountPackets( stream, PT_SetUniform ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetSampler ) );

  std::size_t drawsSeen = 0;
  std::size_t statesSeen = 0;
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    if ( header.type == PT_Draw )
    {
      PacketDraw draw;
      std::memcpy( &draw, stream.Data() + offset, sizeof( draw ) );
      EXPECT_FALSE( draw.indexed );
      EXPECT_EQ( 6u, draw.vertexCount ); // Two triangles.
      ++drawsSeen;
    }
    else if ( header.type == PT_SetRenderState )
    {
      PacketSetRenderState state;
      std::memcpy( &state, stream.Data() + offset, sizeof( state ) );
      EXPECT_TRUE( state.blendingEnabled );
      EXPECT_TRUE( state.depthTestEnabled );
      EXPECT_TRUE( state.depthWriteDisabled ); // Blended particles must not occlude each other.
      ++statesSeen;
    }
    offset += header.size;
  }
  EXPECT_EQ( 2u, drawsSeen );
  EXPECT_EQ( 2u, statesSeen );
}

TEST_F( ParticleSystemTest, EmitsFarthestParticlesFirst )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ParticleGroup* group = system.CreateGroup( "group.json" );
  ASSERT_NE( nullptr, group );
  Particle* nearParticle = group->SpawnParticle();
  Particle* farParticle = group->SpawnParticle();
  ASSERT_NE( nullptr, nearParticle );
  ASSERT_NE( nullptr, farParticle );
  nearParticle->m_position = Vector4( 0.0f, 0.0f, -1.0f, 1.0f );
  farParticle->m_position = Vector4( 0.0f, 0.0f, -10.0f, 1.0f );

  Camera camera;
  camera.SetPosition( Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) );
  camera.SetLookAt( Vector4( 0.0f, 0.0f, -1.0f, 1.0f ) );
  DrawItemList items;
  group->EmitDrawItems( items, &camera );

  ASSERT_EQ( 2u, group->m_particles.size() );
  EXPECT_EQ( farParticle, group->m_particles[0] );
  EXPECT_EQ( nearParticle, group->m_particles[1] );
}

TEST_F( ParticleSystemTest, SceneGraphCompilesParticlesInTheSameFrameList )
{
  ParticleSystem system( fileSystem, renderer, nullptr );
  ASSERT_NE( nullptr, system.CreateEmitter( "emitter.json" ) );
  system.m_emitters.front()->Start();
  system.Update( 0 );
  Camera camera;
  system.SetCamera( &camera );

  SceneGraph graph( renderer );
  ASSERT_TRUE( graph.Initialize() );
  const std::size_t id = graph.AddDrawItemSource( [&system]( DrawItemList& items ) { system.EmitDrawItems( items ); } );

  std::size_t draws = 0;
  EXPECT_CALL( *renderer, ExecuteStream( testing::_ ) )
    .WillOnce( [&draws]( const RenderStream& stream ) { draws = CountPackets( stream, PT_Draw ); } );
  graph.Render( &camera );
  EXPECT_EQ( 2u, draws ); // Particles reach ExecuteStream through the SceneGraph's own compile.

  graph.RemoveDrawItemSource( id );
  EXPECT_CALL( *renderer, ExecuteStream( testing::_ ) )
    .WillOnce( [&draws]( const RenderStream& stream ) { draws = CountPackets( stream, PT_Draw ); } );
  graph.Render( &camera );
  EXPECT_EQ( 0u, draws );
}
