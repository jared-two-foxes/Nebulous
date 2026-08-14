/// @file AlphaQuadBehavioral_test.cpp
/// Behavioral tests for AlphaQuad's InitResources function using a mock RenderSystem.
///
/// These tests verify:
/// - Success path: InitResources returns true when all resources are created successfully
/// - Failure path: InitResources returns false when shader creation fails
/// - Failure path: InitResources returns false when uniform lookup fails
/// - Cleanup: On failure, the output QuadResources struct remains zero-initialized

#include "gtest/gtest.h"

#include "Samples/AlphaQuad/quad_resources.h"

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/Buffer/HardwareBufferImpl.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderStream/RenderStream.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/HardwareShaderImpl.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/Texture.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>
#include <Nebulae/Common/FileSystem/File.h>
#include <Nebulae/Common/FileSystem/FileSystem.h>

#include <functional>
#include <memory>
#include <string>

using namespace Nebulae;

// ============================================================================
// Mock implementations
// ============================================================================

/// A mock File that returns valid data from Read().
class MockFile : public File
{
public:
  bool readCalled = false;

  size_t Read( void* buffer, size_t size ) override
  {
    readCalled = true;
    // Return a minimal valid shader source to satisfy the loading process.
    if ( buffer && size > 0 )
    {
      std::memset( buffer, 0, size );
      return size;
    }
    return 0;
  }
};

/// A mock FileDevice that returns a MockFile.
class MockFileDevice : public FileDevice
{
public:
  File* Open( const std::string& path, FileSystem::Mode mode ) override { return new MockFile(); }

  File* Open( File* file ) override { return file; }

  void Close( File* file ) override { delete file; }
};

// ============================================================================
// Mock Impl classes that return true from Load()
// ============================================================================

class MockShaderImpl : public HardwareShaderImpl
{
public:
  MockShaderImpl( const std::string& name, HardwareShaderType type ) : HardwareShaderImpl( name, type ) {}

  bool Load( File& is ) override { return true; }
  bool Unload() override { return false; }
};

class MockBufferImpl : public HardwareBufferImpl
{
public:
  MockBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags,
                  void* sysMem )
    : HardwareBufferImpl( usage, sizeInBytes, bindFlags, sysMem )
  {
  }

  bool Load() override { return true; }
  bool Unload() override { return false; }
};

class MockInputLayoutImpl : public InputLayoutImpl
{
public:
  MockInputLayoutImpl( const VertexDeceleration* decl, const HardwareShader* shader ) : InputLayoutImpl( decl, shader )
  {
  }

  bool Load() override { return true; }
  bool Unload() override { return false; }
};

class MockTextureImpl : public TextureImpl
{
public:
  MockTextureImpl( const std::string& name ) : TextureImpl( name ) {}

  bool Load( const Image& image ) override { return true; }
  bool LoadFromMemory( unsigned char*, uint8, uint8, int, int ) override { return true; }
  bool Unload() override { return false; }
};

// ============================================================================
// Mock RenderSystem
// ============================================================================

/// Controls which mock behaviors to use.
struct MockConfig
{
  bool failShaderCreate = false;  ///< If true, CreateShaderImpl returns nullptr
  bool failUniformLookup = false; ///< If true, GetUniformImpl returns invalid definition
  bool failBufferCreate = false;  ///< If true, CreateBufferImpl returns nullptr
  bool failLayoutCreate = false;  ///< If true, CreateInputLayoutImpl returns nullptr
  bool failTextureCreate = false; ///< If true, CreateTextureImpl returns nullptr
};

class MockRenderSystem : public RenderSystem
{
public:
  MockConfig config;

  MockRenderSystem() : RenderSystem( std::make_shared<FileSystem>(), WindowPtr() )
  {
    // Mount a mock file device so shader file loading works.
    GetFileSystem()->Mount( "disk", new MockFileDevice() );
  }

  FileSystem* GetFileSystem() { return m_fileSystem.get(); }

  // --- Override Initiate to set up factories ---
  bool Initiate() override
  {
    RenderSystem::Initiate();
    return true;
  }

  // --- Override Create*Impl methods ---

  HardwareShaderImpl* CreateShaderImpl( const std::string& name, HardwareShaderType type ) override
  {
    if ( config.failShaderCreate )
      return nullptr;
    return new MockShaderImpl( name, type );
  }

  HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes,
                                        HardwareBufferBinding bindFlags, void* sysMem ) override
  {
    if ( config.failBufferCreate )
      return nullptr;
    return new MockBufferImpl( usage, sizeInBytes, bindFlags, sysMem );
  }

  InputLayoutImpl* CreateInputLayoutImpl( const VertexDeceleration* decl, const HardwareShader* shader ) override
  {
    if ( config.failLayoutCreate )
      return nullptr;
    return new MockInputLayoutImpl( decl, shader );
  }

  TextureImpl* CreateTextureImpl( const std::string& name ) override
  {
    if ( config.failTextureCreate )
      return nullptr;
    return new MockTextureImpl( name );
  }

  // --- Override pure virtuals ---

  const std::string& GetName() const override
  {
    static const std::string name = "MockRenderSystem";
    return name;
  }

  void Clear() override {}

  bool ReflectProgram( ProgramObject&, UniformDefinitionMap& ) override { return true; }
  bool ReflectProgramShaders( HardwareShader*, HardwareShader*, UniformDefinitionMap& ) override { return true; }

  void ExecuteStream( const RenderStream& ) override {}

  UniformDefinitionBase GetUniformImpl( const char* name ) const override
  {
    if ( config.failUniformLookup )
    {
      // Return invalid definition (default-constructed has logicalIndex = -1)
      return UniformDefinitionBase();
    }

    // Return a valid definition for "s_texture".
    UniformDefinitionBase def;
    def.type = UT_SAMPLER2D;
    def.logicalIndex = 0;
    def.physicalIndex = 0;
    def.elementSize = 1;
    def.arraySize = 1;
    return def;
  }

  void SetUniformImpl( const UniformDefinition<float>&, const float& ) override {}
  void SetUniformImpl( const UniformDefinition<int32>&, const int32& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector2>&, const Vector2& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector4>&, const Vector4& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix3>&, const Matrix3& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix4>&, const Matrix4& ) override {}
  void SetUniformImpl( const UniformDefinition<Texture*>&, const Texture* ) override {}
};

// ============================================================================
// Tests
// ============================================================================

TEST( AlphaQuadBehavioral, InitResources_Success )
{
  MockRenderSystem rs;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  EXPECT_TRUE( InitResources( &rs, res ) );

  // Verify resources are populated.
  EXPECT_NE( nullptr, res.vertexShader );
  EXPECT_NE( nullptr, res.pixelShader );
  EXPECT_NE( nullptr, res.vertexBuffer );
  EXPECT_NE( nullptr, res.inputLayout );
  EXPECT_NE( nullptr, res.texture );
  EXPECT_TRUE( res.uniformDef.IsValid() );
}

TEST( AlphaQuadBehavioral, InitResources_Failure_ShaderCreate )
{
  MockRenderSystem rs;
  rs.config.failShaderCreate = true;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  // Fill res with non-zero values to verify it is left unchanged on failure
  // (the implementation uses a temporary struct and only commits on success).
  std::memset( &res, 0xFF, sizeof( res ) );

  EXPECT_FALSE( InitResources( &rs, res ) );

  // Verify res is unchanged (still has 0xFF pattern) — the temporary struct
  // approach ensures the output is never partially modified.
  EXPECT_NE( nullptr, res.vertexShader );
  EXPECT_NE( nullptr, res.pixelShader );
  EXPECT_NE( nullptr, res.vertexBuffer );
  EXPECT_NE( nullptr, res.inputLayout );
  EXPECT_NE( nullptr, res.texture );
}

TEST( AlphaQuadBehavioral, InitResources_Failure_UniformLookup )
{
  MockRenderSystem rs;
  rs.config.failUniformLookup = true;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  // Fill with 0xFF to verify res is left unchanged on failure.
  std::memset( &res, 0xFF, sizeof( res ) );

  EXPECT_FALSE( InitResources( &rs, res ) );

  // Verify res is unchanged — the temporary struct approach ensures the output
  // is never partially modified.
  EXPECT_NE( nullptr, res.vertexShader );
  EXPECT_NE( nullptr, res.pixelShader );
  EXPECT_NE( nullptr, res.vertexBuffer );
  EXPECT_NE( nullptr, res.inputLayout );
  EXPECT_NE( nullptr, res.texture );
}

TEST( AlphaQuadBehavioral, InitResources_Failure_BufferCreate )
{
  MockRenderSystem rs;
  rs.config.failBufferCreate = true;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  // Fill with 0xFF to verify res is left unchanged on failure.
  std::memset( &res, 0xFF, sizeof( res ) );

  EXPECT_FALSE( InitResources( &rs, res ) );

  // Verify res is unchanged — the temporary struct approach ensures the output
  // is never partially modified.
  EXPECT_NE( nullptr, res.vertexShader );
  EXPECT_NE( nullptr, res.pixelShader );
  EXPECT_NE( nullptr, res.vertexBuffer );
  EXPECT_NE( nullptr, res.inputLayout );
  EXPECT_NE( nullptr, res.texture );
}

TEST( AlphaQuadBehavioral, UniformDefinition_IsValid_DetectsInvalid )
{
  UniformDefinition<Texture*> def; // default: logicalIndex = -1
  EXPECT_FALSE( def.IsValid() );

  UniformDefinition<Texture*> valid;
  valid.logicalIndex = 0;
  EXPECT_TRUE( valid.IsValid() );
}

TEST( AlphaQuadBehavioral, BuildQuadStream_ProducesNonEmptyStream )
{
  // BuildQuadStream doesn't require a RenderSystem, just a populated QuadResources.
  // We can verify it produces a non-empty stream with valid packet data.

  // Create minimal valid resources (just need non-null pointers, not actually loaded).
  MockRenderSystem rs;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  ASSERT_TRUE( InitResources( &rs, res ) );

  RenderStream stream = BuildQuadStream( res );
  EXPECT_GT( stream.Size(), 0u );

  // Verify the stream starts with a PT_SetProgram packet.
  const uint8_t* data = stream.Data();
  ASSERT_NE( nullptr, data );
  const PacketHeader* header = reinterpret_cast<const PacketHeader*>( data );
  EXPECT_EQ( PT_SetProgram, header->type );
}

TEST( AlphaQuadBehavioral, BuildQuadStream_SemanticConsistency )
{
  // Verify packet-level semantic consistency: the stream must use a non-indexed
  // draw (no index buffer) with a vertex count that matches the vertex buffer size.
  //
  // This catches regressions where the draw command type doesn't match the geometry
  // setup (e.g., glDrawArrays with vertexCount > actual vertex count).

  MockRenderSystem rs;
  ASSERT_TRUE( rs.Initiate() );

  QuadResources res = {};
  ASSERT_TRUE( InitResources( &rs, res ) );

  RenderStream stream = BuildQuadStream( res );
  ASSERT_GT( stream.Size(), 0u );

  // Parse the stream to find PT_SetGeometry and PT_Draw packets.
  const uint8_t* data = stream.Data();
  std::size_t offset = 0;
  bool foundGeometry = false;
  bool foundDraw = false;

  while ( offset < stream.Size() )
  {
    const PacketHeader* header = reinterpret_cast<const PacketHeader*>( data + offset );
    ASSERT_NE( nullptr, header );
    ASSERT_GT( header->size, 0u );

    switch ( header->type )
    {
    case PT_SetGeometry:
    {
      const PacketSetGeometry* geo = reinterpret_cast<const PacketSetGeometry*>( data + offset );
      // Non-indexed draw: index buffer must be nullptr.
      EXPECT_EQ( nullptr, geo->indexBuffer ) << "PT_SetGeometry must not bind an index buffer for non-indexed draw.";
      foundGeometry = true;
      break;
    }
    case PT_Draw:
    {
      const PacketDraw* draw = reinterpret_cast<const PacketDraw*>( data + offset );
      // With 6 vertices and no index buffer, vertexCount must be 6.
      EXPECT_EQ( 6u, draw->vertexCount )
        << "PT_Draw vertexCount must match the number of vertices (6) for non-indexed quad.";
      foundDraw = true;
      break;
    }
    default:
      break;
    }

    offset += header->size;
  }

  EXPECT_TRUE( foundGeometry ) << "Stream must contain a PT_SetGeometry packet.";
  EXPECT_TRUE( foundDraw ) << "Stream must contain a PT_Draw packet.";
}

TEST( AlphaQuadBehavioral, InitResources_RetryAfterFailure )
{
  // Verify that a failed InitResources followed by a successful retry works correctly.
  // This tests that rollback/cleanup on failure properly resets resource state so that
  // the factory can re-initialize them on the next attempt.
  MockRenderSystem rs;
  ASSERT_TRUE( rs.Initiate() );

  // First attempt: fail at buffer creation (vertex shader and pixel shader are created).
  rs.config.failBufferCreate = true;
  QuadResources res = {};
  EXPECT_FALSE( InitResources( &rs, res ) );

  // Second attempt: succeed.
  rs.config.failBufferCreate = false;
  EXPECT_TRUE( InitResources( &rs, res ) );

  // Verify all resources are populated after successful retry.
  EXPECT_NE( nullptr, res.vertexShader );
  EXPECT_NE( nullptr, res.pixelShader );
  EXPECT_NE( nullptr, res.vertexBuffer );
  EXPECT_NE( nullptr, res.inputLayout );
  EXPECT_NE( nullptr, res.texture );
  EXPECT_TRUE( res.uniformDef.IsValid() );
}

TEST( AlphaQuadBehavioral, InitResources_CleanupOnFailure )
{
  // Verify that resources created before a failure point are properly unloaded
  // and their impls are released, so no GPU/engine resources are retained.
  MockRenderSystem rs;
  ASSERT_TRUE( rs.Initiate() );

  // Fail at buffer creation — vertex shader and pixel shader should have been
  // created successfully before the failure.
  rs.config.failBufferCreate = true;
  QuadResources res = {};
  EXPECT_FALSE( InitResources( &rs, res ) );

  // Verify the shaders that were created are now in unloaded state.
  HardwareShader* vs = rs.FindShaderByName( "textured_quad_vs.glsl" );
  HardwareShader* ps = rs.FindShaderByName( "textured_quad_ps.glsl" );
  ASSERT_NE( nullptr, vs );
  ASSERT_NE( nullptr, ps );
  EXPECT_FALSE( vs->IsLoaded() );
  EXPECT_FALSE( ps->IsLoaded() );

  // Verify their impls were deleted (GetImpl returns null after cleanup).
  EXPECT_EQ( nullptr, vs->GetImpl() );
  EXPECT_EQ( nullptr, ps->GetImpl() );
}

TEST( AlphaQuadBehavioral, PostFailure_IsLoadedImplConsistency )
{
  // Validate that after any failure path, no resource is left in the inconsistent
  // state of IsLoaded()==true with GetImpl()==nullptr. This matches real engine
  // semantics where Unload() returns false and the rollback code must explicitly
  // reset the loading status.

  // Test failure at each possible failure point.
  struct FailureCase
  {
    const char* name;
    std::function<void( MockConfig& )> setup;
  };

  FailureCase cases[] = {
    { "shader create", []( MockConfig& c ) { c.failShaderCreate = true; } },
    { "buffer create", []( MockConfig& c ) { c.failBufferCreate = true; } },
    { "layout create", []( MockConfig& c ) { c.failLayoutCreate = true; } },
    { "texture create", []( MockConfig& c ) { c.failTextureCreate = true; } },
    { "uniform lookup", []( MockConfig& c ) { c.failUniformLookup = true; } },
  };

  for ( auto& testCase : cases )
  {
    MockRenderSystem rs;
    rs.config = MockConfig{};
    testCase.setup( rs.config );
    ASSERT_TRUE( rs.Initiate() );

    QuadResources res = {};
    EXPECT_FALSE( InitResources( &rs, res ) ) << "Expected failure for case: " << testCase.name;

    // Check all named resources in the render system for the invariant:
    // If IsLoaded() is true, GetImpl() must not be nullptr.
    auto checkShader = [&]( const std::string& name )
    {
      HardwareShader* s = rs.FindShaderByName( name );
      if ( s )
      {
        if ( s->IsLoaded() )
        {
          EXPECT_NE( nullptr, s->GetImpl() ) << "Case '" << testCase.name << "': shader '" << name
                                             << "' has IsLoaded()==true but GetImpl()==nullptr (inconsistent state)";
        }
      }
    };

    checkShader( "textured_quad_vs.glsl" );
    checkShader( "textured_quad_ps.glsl" );
  }
}
