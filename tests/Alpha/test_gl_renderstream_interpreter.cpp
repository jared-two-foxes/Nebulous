#include <gtest/gtest.h>

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>
#include <Nebulae/Alpha/RenderStream/RenderStream.h>
#include <Nebulae/Alpha/Shaders/UniformTypeHelpers.h>

// Test-only private access for white-box validation of program cache behavior.
#define private public
#include <Includes/HardwareShaderImpl_OGL.h>
#include <Includes/ProgramObject.h>
#include <Includes/RenderSystem_OGL.h>
#include <Includes/TextureImpl_OGL.h>
#undef private

#include <Nebulae/Common/Log.h>

#include <fstream>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace
{

using namespace Nebulae;

// Test-local GL function pointers (mocked for testing)
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;

PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;

PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
typedef void( APIENTRY* PFNGLBINDTEXTUREPROC )( GLenum, GLuint );
PFNGLBINDTEXTUREPROC glBindTexture = nullptr;

struct GLUniformCallState
{
  int uniform1fvCalls = 0;
  int uniform2fvCalls = 0;
  int uniform3fvCalls = 0;
  int uniform4fvCalls = 0;
  int uniform1ivCalls = 0;
  int uniform2ivCalls = 0;
  int uniform3ivCalls = 0;
  int uniform4ivCalls = 0;
  int matrix3Calls = 0;
  int matrix4Calls = 0;
  int uniform1iCalls = 0;

  int lastLocation = -1;
  int lastCount = -1;
  int lastSamplerUnit = -1;
};

GLUniformCallState g_uniformCalls;
std::vector<std::string> g_logMessages;

void APIENTRY StubUniform1fv( GLint location, GLsizei count, const GLfloat* )
{
  ++g_uniformCalls.uniform1fvCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform2fv( GLint location, GLsizei count, const GLfloat* )
{
  ++g_uniformCalls.uniform2fvCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform3fv( GLint location, GLsizei count, const GLfloat* )
{
  ++g_uniformCalls.uniform3fvCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform4fv( GLint location, GLsizei count, const GLfloat* )
{
  ++g_uniformCalls.uniform4fvCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform1iv( GLint location, GLsizei count, const GLint* )
{
  ++g_uniformCalls.uniform1ivCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform2iv( GLint location, GLsizei count, const GLint* )
{
  ++g_uniformCalls.uniform2ivCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform3iv( GLint location, GLsizei count, const GLint* )
{
  ++g_uniformCalls.uniform3ivCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform4iv( GLint location, GLsizei count, const GLint* )
{
  ++g_uniformCalls.uniform4ivCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniformMatrix3fv( GLint location, GLsizei count, GLboolean, const GLfloat* )
{
  ++g_uniformCalls.matrix3Calls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniformMatrix4fv( GLint location, GLsizei count, GLboolean, const GLfloat* )
{
  ++g_uniformCalls.matrix4Calls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastCount = count;
}
void APIENTRY StubUniform1i( GLint location, GLint value )
{
  ++g_uniformCalls.uniform1iCalls;
  g_uniformCalls.lastLocation = location;
  g_uniformCalls.lastSamplerUnit = value;
}

GLuint APIENTRY StubCreateProgram() { return 77u; }
void APIENTRY StubAttachShader( GLuint, GLuint ) {}
void APIENTRY StubDeleteProgram( GLuint ) {}
void APIENTRY StubDetachShader( GLuint, GLuint ) {}
void APIENTRY StubLinkProgram( GLuint ) {}
void APIENTRY StubUseProgram( GLuint ) {}
void APIENTRY StubGetProgramInfoLog( GLuint, GLsizei, GLsizei*, GLchar* ) {}
void APIENTRY StubGetActiveUniform( GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar* ) {}
GLint APIENTRY StubGetUniformLocation( GLuint, const GLchar* ) { return 0; }
void APIENTRY StubActiveTexture( GLenum ) {}
void APIENTRY StubBindTexture( GLenum, GLuint ) {}
void APIENTRY StubGetProgramiv( GLuint, GLenum pname, GLint* params )
{
  if ( pname == GL_LINK_STATUS )
  {
    *params = GL_TRUE;
    return;
  }

  if ( pname == GL_ACTIVE_UNIFORMS )
  {
    *params = 0;
    return;
  }

  if ( pname == GL_INFO_LOG_LENGTH )
  {
    *params = 0;
    return;
  }

  *params = 0;
}

void CaptureLog( const char* message )
{
  if ( message != nullptr )
  {
    g_logMessages.emplace_back( message );
  }
}

template <typename T> T MakePacket( std::uint16_t packetType )
{
  T packet{};
  packet.header.type = packetType;
  packet.header.size = static_cast<std::uint16_t>( sizeof( T ) );
  return packet;
}

std::size_t AppendUniformPacket( RenderStream& stream, const UniformWrite& write,
                                 const std::vector<std::uint8_t>& payload )
{
  PacketSetUniform p = MakePacket<PacketSetUniform>( PT_SetUniform );
  p.write = write;
  const std::size_t oldSize = stream.Size();
  stream.Write( p, payload.size() );

  if ( !payload.empty() )
  {
    std::uint8_t* mutableData = const_cast<std::uint8_t*>( stream.Data() );
    std::memcpy( mutableData + oldSize + sizeof( PacketSetUniform ), payload.data(), payload.size() );
  }

  return oldSize;
}

class ProgramObjectProbe : public ProgramObject
{
public:
  ProgramObjectProbe() : ProgramObject( nullptr, nullptr ) {}

  void SetUniform( const std::string& name, const UniformDefinitionBase& value ) { m_uniformDefinitions[name] = value; }
};

class TextureImplProbe : public TextureImpl_OGL
{
public:
  explicit TextureImplProbe( GLuint handle ) : TextureImpl_OGL( "test" ) { m_iHandle = handle; }
};

class ShaderImplProbe : public HardwareShaderImpl_OGL
{
public:
  ShaderImplProbe( HardwareShaderType type, GLuint handle ) : HardwareShaderImpl_OGL( "test", type )
  {
    m_iHandle = handle;
  }
};

class RenderSystemOGLStreamSpy : public RenderSystem_OGL
{
public:
  using RenderSystem_OGL::RenderSystem_OGL;

  int setShadersCalls = 0;
  int setVertexCalls = 0;
  int setIndexCalls = 0;
  int setLayoutCalls = 0;
  int setBlendCalls = 0;
  int setDepthCalls = 0;
  int setClearCalls = 0;
  int drawCalls = 0;
  std::size_t lastDrawCount = 0;
  std::size_t lastDrawStart = 0;

  HardwareShader* lastVS = nullptr;
  HardwareShader* lastFS = nullptr;
  HardwareBuffer* lastVB = nullptr;
  HardwareBuffer* lastIB = nullptr;
  InputLayout* lastLayout = nullptr;
  std::size_t lastStride = 0;
  std::size_t lastOffset = 0;
  bool lastBlendEnabled = false;
  bool lastDepthEnabled = false;
  float lastClear[4] = {};

  void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader ) override
  {
    ++setShadersCalls;
    lastVS = vertexShader;
    lastFS = fragmentShader;
  }

  void SetVertexBuffers( int iSlot, HardwareBuffer* pBuffers, std::size_t iStride, std::size_t iOffset ) override
  {
    (void)iSlot;
    ++setVertexCalls;
    lastVB = pBuffers;
    lastStride = iStride;
    lastOffset = iOffset;
  }

  void SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset ) override
  {
    ++setIndexCalls;
    lastIB = pBuffer;
    lastOffset = iOffset;
  }

  void SetInputLayout( InputLayout* pInputLayout ) override
  {
    ++setLayoutCalls;
    lastLayout = pInputLayout;
  }

  void SetBlendingState( bool enable ) override
  {
    ++setBlendCalls;
    lastBlendEnabled = enable;
  }

  void SetDepthTest( bool enable ) override
  {
    ++setDepthCalls;
    lastDepthEnabled = enable;
  }

  void SetClearColour( float r, float g, float b, float a ) override
  {
    ++setClearCalls;
    lastClear[0] = r;
    lastClear[1] = g;
    lastClear[2] = b;
    lastClear[3] = a;
  }

  void Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation ) override
  {
    ++drawCalls;
    lastDrawCount = iVertexCount;
    lastDrawStart = iStartVertexLocation;
  }

  // Pure virtual method stubs (not tested, just needed for instantiation)
  HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>&, std::size_t, HardwareBufferBinding,
                                        void* ) override
  {
    return nullptr;
  }
  HardwareShaderImpl* CreateShaderImpl( const std::string&, HardwareShaderType ) override { return nullptr; }
  InputLayoutImpl* CreateInputLayoutImpl( const VertexDeceleration*, const HardwareShader* ) override
  {
    return nullptr;
  }
  Sampler::Impl* CreateSamplerImpl() override { return nullptr; }
  TextureImpl* CreateTextureImpl( const std::string& ) override { return nullptr; }
  UniformDefinitionBase GetUniformImpl( const char* ) const override { return UniformDefinitionBase{}; }
  void SetUniformImpl( const UniformDefinition<float>&, const float& ) override {}
  void SetUniformImpl( const UniformDefinition<int>&, const int& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector2>&, const Vector2& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector4>&, const Vector4& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix3>&, const Matrix3& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix4>&, const Matrix4& ) override {}
  void SetUniformImpl( const UniformDefinition<Texture*>&, const Texture* ) override {}
};

class GLRenderStreamInterpreterTest : public ::testing::Test
{
protected:
  PFNGLUNIFORM1FVPROC oldUniform1fv = nullptr;
  PFNGLUNIFORM2FVPROC oldUniform2fv = nullptr;
  PFNGLUNIFORM3FVPROC oldUniform3fv = nullptr;
  PFNGLUNIFORM4FVPROC oldUniform4fv = nullptr;
  PFNGLUNIFORM1IVPROC oldUniform1iv = nullptr;
  PFNGLUNIFORM2IVPROC oldUniform2iv = nullptr;
  PFNGLUNIFORM3IVPROC oldUniform3iv = nullptr;
  PFNGLUNIFORM4IVPROC oldUniform4iv = nullptr;
  PFNGLUNIFORMMATRIX3FVPROC oldUniformMatrix3fv = nullptr;
  PFNGLUNIFORMMATRIX4FVPROC oldUniformMatrix4fv = nullptr;
  PFNGLUNIFORM1IPROC oldUniform1i = nullptr;

  PFNGLATTACHSHADERPROC oldAttachShader = nullptr;
  PFNGLCREATEPROGRAMPROC oldCreateProgram = nullptr;
  PFNGLDELETEPROGRAMPROC oldDeleteProgram = nullptr;
  PFNGLDETACHSHADERPROC oldDetachShader = nullptr;
  PFNGLGETPROGRAMIVPROC oldGetProgramiv = nullptr;
  PFNGLGETPROGRAMINFOLOGPROC oldGetProgramInfoLog = nullptr;
  PFNGLLINKPROGRAMPROC oldLinkProgram = nullptr;
  PFNGLUSEPROGRAMPROC oldUseProgram = nullptr;
  PFNGLGETACTIVEUNIFORMPROC oldGetActiveUniform = nullptr;
  PFNGLGETUNIFORMLOCATIONPROC oldGetUniformLocation = nullptr;

  PFNGLACTIVETEXTUREPROC oldActiveTexture = nullptr;
  PFNGLBINDTEXTUREPROC oldBindTexture = nullptr;

  void SetUp() override
  {
    g_uniformCalls = GLUniformCallState{};
    g_logMessages.clear();

    oldUniform1fv = glUniform1fv;
    oldUniform2fv = glUniform2fv;
    oldUniform3fv = glUniform3fv;
    oldUniform4fv = glUniform4fv;
    oldUniform1iv = glUniform1iv;
    oldUniform2iv = glUniform2iv;
    oldUniform3iv = glUniform3iv;
    oldUniform4iv = glUniform4iv;
    oldUniformMatrix3fv = glUniformMatrix3fv;
    oldUniformMatrix4fv = glUniformMatrix4fv;
    oldUniform1i = glUniform1i;

    oldAttachShader = glAttachShader;
    oldCreateProgram = glCreateProgram;
    oldDeleteProgram = glDeleteProgram;
    oldDetachShader = glDetachShader;
    oldGetProgramiv = glGetProgramiv;
    oldGetProgramInfoLog = glGetProgramInfoLog;
    oldLinkProgram = glLinkProgram;
    oldUseProgram = glUseProgram;
    oldGetActiveUniform = glGetActiveUniform;
    oldGetUniformLocation = glGetUniformLocation;
    oldActiveTexture = glActiveTexture;
    oldBindTexture = glBindTexture;

    glUniform1fv = &StubUniform1fv;
    glUniform2fv = &StubUniform2fv;
    glUniform3fv = &StubUniform3fv;
    glUniform4fv = &StubUniform4fv;
    glUniform1iv = &StubUniform1iv;
    glUniform2iv = &StubUniform2iv;
    glUniform3iv = &StubUniform3iv;
    glUniform4iv = &StubUniform4iv;
    glUniformMatrix3fv = &StubUniformMatrix3fv;
    glUniformMatrix4fv = &StubUniformMatrix4fv;
    glUniform1i = &StubUniform1i;

    glAttachShader = &StubAttachShader;
    glCreateProgram = &StubCreateProgram;
    glDeleteProgram = &StubDeleteProgram;
    glDetachShader = &StubDetachShader;
    glGetProgramiv = &StubGetProgramiv;
    glGetProgramInfoLog = &StubGetProgramInfoLog;
    glLinkProgram = &StubLinkProgram;
    glUseProgram = &StubUseProgram;
    glGetActiveUniform = &StubGetActiveUniform;
    glGetUniformLocation = &StubGetUniformLocation;
    glActiveTexture = &StubActiveTexture;
    glBindTexture = &StubBindTexture;

    SetLogCallback( &CaptureLog );
  }

  void TearDown() override
  {
    glUniform1fv = oldUniform1fv;
    glUniform2fv = oldUniform2fv;
    glUniform3fv = oldUniform3fv;
    glUniform4fv = oldUniform4fv;
    glUniform1iv = oldUniform1iv;
    glUniform2iv = oldUniform2iv;
    glUniform3iv = oldUniform3iv;
    glUniform4iv = oldUniform4iv;
    glUniformMatrix3fv = oldUniformMatrix3fv;
    glUniformMatrix4fv = oldUniformMatrix4fv;
    glUniform1i = oldUniform1i;

    glAttachShader = oldAttachShader;
    glCreateProgram = oldCreateProgram;
    glDeleteProgram = oldDeleteProgram;
    glDetachShader = oldDetachShader;
    glGetProgramiv = oldGetProgramiv;
    glGetProgramInfoLog = oldGetProgramInfoLog;
    glLinkProgram = oldLinkProgram;
    glUseProgram = oldUseProgram;
    glGetActiveUniform = oldGetActiveUniform;
    glGetUniformLocation = oldGetUniformLocation;
    glActiveTexture = oldActiveTexture;
    glBindTexture = oldBindTexture;

    SetLogCallback( nullptr );
  }
};

TEST( GLRenderStreamInterpreterContract, RenderSystemOGLMustBeConcrete )
{
  EXPECT_FALSE( std::is_abstract_v<RenderSystem_OGL> )
    << "RenderSystem_OGL must implement ReflectProgram + ExecuteStream for SA-442.";
}

TEST_F( GLRenderStreamInterpreterTest, EmptyStreamIsNoOp )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystemOGLStreamSpy rs( nullptr, nullptr );
    RenderStream stream;

    rs.ExecuteStream( stream );

    EXPECT_EQ( 0, rs.setShadersCalls );
    EXPECT_EQ( 0, rs.drawCalls );
    EXPECT_EQ( 0, rs.setBlendCalls );
  }
}

TEST_F( GLRenderStreamInterpreterTest, HeaderWalkDispatchesAndSkipsUnknownPacketByHeaderSize )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystemOGLStreamSpy rs( nullptr, nullptr );
    RenderStream stream;

    struct UnknownPacket
    {
      PacketHeader header;
      std::uint32_t payload;
    };

    UnknownPacket unknown{};
    unknown.header.type = 0x7FFFu;
    unknown.header.size = static_cast<std::uint16_t>( sizeof( UnknownPacket ) );
    unknown.payload = 0xAABBCCDDu;
    stream.Write( unknown );

    PacketDraw draw = MakePacket<PacketDraw>( PT_Draw );
    draw.vertexCount = 11;
    draw.startVertex = 3;
    stream.Write( draw );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 1, rs.drawCalls );
    EXPECT_EQ( 11u, rs.lastDrawCount );
    EXPECT_EQ( 3u, rs.lastDrawStart );
    EXPECT_FALSE( g_logMessages.empty() ) << "Unknown packet should log a warning and continue.";
  }
}

TEST_F( GLRenderStreamInterpreterTest, DispatchesSetGeometryAndSetRenderStatePackets )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystemOGLStreamSpy rs( nullptr, nullptr );
    RenderStream stream;

    HardwareBuffer vb( "vb", nullptr );
    HardwareBuffer ib( "ib", nullptr );
    InputLayout layout( "layout", nullptr );

    PacketSetGeometry geometry = MakePacket<PacketSetGeometry>( PT_SetGeometry );
    geometry.vertexBuffer = &vb;
    geometry.indexBuffer = &ib;
    geometry.inputLayout = &layout;
    geometry.stride = 24u;
    geometry.offset = 8u;
    stream.Write( geometry );

    PacketSetRenderState rsPacket = MakePacket<PacketSetRenderState>( PT_SetRenderState );
    rsPacket.blendingEnabled = true;
    rsPacket.depthTestEnabled = false;
    rsPacket.clearColourR = 0.25f;
    rsPacket.clearColourG = 0.5f;
    rsPacket.clearColourB = 0.75f;
    rsPacket.clearColourA = 1.0f;
    stream.Write( rsPacket );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 1, rs.setVertexCalls );
    EXPECT_EQ( &vb, rs.lastVB );
    EXPECT_EQ( 24u, rs.lastStride );
    EXPECT_EQ( 1, rs.setIndexCalls );
    EXPECT_EQ( &ib, rs.lastIB );
    EXPECT_EQ( 1, rs.setLayoutCalls );
    EXPECT_EQ( &layout, rs.lastLayout );

    EXPECT_EQ( 1, rs.setBlendCalls );
    EXPECT_TRUE( rs.lastBlendEnabled );
    EXPECT_EQ( 1, rs.setDepthCalls );
    EXPECT_FALSE( rs.lastDepthEnabled );
    EXPECT_EQ( 1, rs.setClearCalls );
    EXPECT_FLOAT_EQ( 0.25f, rs.lastClear[0] );
    EXPECT_FLOAT_EQ( 0.5f, rs.lastClear[1] );
    EXPECT_FLOAT_EQ( 0.75f, rs.lastClear[2] );
    EXPECT_FLOAT_EQ( 1.0f, rs.lastClear[3] );
  }
}

TEST_F( GLRenderStreamInterpreterTest, SetProgramPacketCreatesOrFindsProgramAndUsesIt )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );

    HardwareShader vertex( "vs", nullptr );
    HardwareShader fragment( "fs", nullptr );
    vertex.SetImpl( new ShaderImplProbe( VERTEX_SHADER, 101u ) );
    fragment.SetImpl( new ShaderImplProbe( PIXEL_SHADER, 202u ) );

    RenderStream stream;
    PacketSetProgram packet = MakePacket<PacketSetProgram>( PT_SetProgram );
    packet.vertexShader = &vertex;
    packet.fragmentShader = &fragment;
    stream.Write( packet );

    rs.ExecuteStream( stream );

    ASSERT_NE( nullptr, rs.m_boundProgram );
    EXPECT_EQ( 1u, rs.m_programObjects.size() );

    ProgramObject* first = rs.m_boundProgram;
    rs.ExecuteStream( stream );
    EXPECT_EQ( 1u, rs.m_programObjects.size() ) << "Shader pair should reuse existing ProgramObject.";
    EXPECT_EQ( first, rs.m_boundProgram );
  }
}

TEST_F( GLRenderStreamInterpreterTest, UniformDispatchUsesUniformTypeNotElementCount )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );
    RenderStream stream;

    const float matrix2x2Payload[4] = { 1.f, 2.f, 3.f, 4.f };
    const float float4Payload[4] = { 5.f, 6.f, 7.f, 8.f };

    UniformWrite matrixLike{};
    matrixLike.gpuLocation = 17;
    matrixLike.type = UT_MATRIX_3X3;
    matrixLike.arraySize = 1;
    matrixLike.payloadBytes = static_cast<std::uint16_t>( sizeof( matrix2x2Payload ) );

    UniformWrite vecLike{};
    vecLike.gpuLocation = 18;
    vecLike.type = UT_FLOAT4;
    vecLike.arraySize = 1;
    vecLike.payloadBytes = static_cast<std::uint16_t>( sizeof( float4Payload ) );

    AppendUniformPacket( stream, matrixLike,
                         std::vector<std::uint8_t>(
                           reinterpret_cast<const std::uint8_t*>( matrix2x2Payload ),
                           reinterpret_cast<const std::uint8_t*>( matrix2x2Payload ) + sizeof( matrix2x2Payload ) ) );
    AppendUniformPacket(
      stream, vecLike,
      std::vector<std::uint8_t>( reinterpret_cast<const std::uint8_t*>( float4Payload ),
                                 reinterpret_cast<const std::uint8_t*>( float4Payload ) + sizeof( float4Payload ) ) );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 1, g_uniformCalls.matrix3Calls ) << "UT_MATRIX_3X3 must route to glUniformMatrix3fv.";
    EXPECT_EQ( 1, g_uniformCalls.uniform4fvCalls ) << "UT_FLOAT4 must route to glUniform4fv.";
  }
}

TEST_F( GLRenderStreamInterpreterTest, UniformDispatchCoversScalarIntVectorAndMatrixTypes )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );
    RenderStream stream;

    auto appendFloat = [&]( UniformType type, int location, std::size_t count )
    {
      std::vector<float> payload( GetUniformElementCount( type ) * count, 1.0f );
      UniformWrite write{};
      write.gpuLocation = location;
      write.type = type;
      write.arraySize = static_cast<std::uint16_t>( count );
      write.payloadBytes = static_cast<std::uint16_t>( payload.size() * sizeof( float ) );
      AppendUniformPacket( stream, write,
                           std::vector<std::uint8_t>( reinterpret_cast<const std::uint8_t*>( payload.data() ),
                                                      reinterpret_cast<const std::uint8_t*>( payload.data() ) +
                                                        payload.size() * sizeof( float ) ) );
    };

    auto appendInt = [&]( UniformType type, int location, std::size_t count )
    {
      std::vector<std::int32_t> payload( GetUniformElementCount( type ) * count, 7 );
      UniformWrite write{};
      write.gpuLocation = location;
      write.type = type;
      write.arraySize = static_cast<std::uint16_t>( count );
      write.payloadBytes = static_cast<std::uint16_t>( payload.size() * sizeof( std::int32_t ) );
      AppendUniformPacket( stream, write,
                           std::vector<std::uint8_t>( reinterpret_cast<const std::uint8_t*>( payload.data() ),
                                                      reinterpret_cast<const std::uint8_t*>( payload.data() ) +
                                                        payload.size() * sizeof( std::int32_t ) ) );
    };

    appendFloat( UT_FLOAT1, 1, 1 );
    appendFloat( UT_FLOAT2, 2, 1 );
    appendFloat( UT_FLOAT3, 3, 1 );
    appendFloat( UT_FLOAT4, 4, 1 );
    appendInt( UT_INT1, 11, 1 );
    appendInt( UT_INT2, 12, 1 );
    appendInt( UT_INT3, 13, 1 );
    appendInt( UT_INT4, 14, 1 );
    appendFloat( UT_MATRIX_3X3, 21, 1 );
    appendFloat( UT_MATRIX_4X4, 22, 1 );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 1, g_uniformCalls.uniform1fvCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform2fvCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform3fvCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform4fvCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform1ivCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform2ivCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform3ivCalls );
    EXPECT_EQ( 1, g_uniformCalls.uniform4ivCalls );
    EXPECT_EQ( 1, g_uniformCalls.matrix3Calls );
    EXPECT_EQ( 1, g_uniformCalls.matrix4Calls );
  }
}

TEST_F( GLRenderStreamInterpreterTest, SetSamplerNullTextureLogsWarningAndDoesNotCrash )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );
    RenderStream stream;

    PacketSetSampler packet = MakePacket<PacketSetSampler>( PT_SetSampler );
    packet.write.gpuLocation = 9;
    packet.write.unit = 3;
    packet.write.tex = nullptr;
    stream.Write( packet );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 0, g_uniformCalls.uniform1iCalls );
    EXPECT_FALSE( g_logMessages.empty() ) << "Null texture should emit warning log.";
  }
}

TEST_F( GLRenderStreamInterpreterTest, SetSamplerUsesGpuLocationAndTextureUnit )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );
    RenderStream stream;

    Texture texture( "tex", nullptr );
    texture.SetImpl( new TextureImplProbe( 123u ) );

    PacketSetSampler packet = MakePacket<PacketSetSampler>( PT_SetSampler );
    packet.write.gpuLocation = 33;
    packet.write.unit = 5;
    packet.write.tex = &texture;
    stream.Write( packet );

    rs.ExecuteStream( stream );

    EXPECT_EQ( 1, g_uniformCalls.uniform1iCalls );
    EXPECT_EQ( 33, g_uniformCalls.lastLocation );
    EXPECT_EQ( 5, g_uniformCalls.lastSamplerUnit );
  }
}

TEST_F( GLRenderStreamInterpreterTest, ReflectProgramCopiesExistingUniformDefinitionMap )
{
  if constexpr ( std::is_abstract_v<RenderSystem_OGL> )
  {
    GTEST_SKIP() << "RenderSystem_OGL is abstract until ExecuteStream/ReflectProgram are implemented.";
  }
  else
  {
    RenderSystem_OGL rs( nullptr, nullptr );
    ProgramObjectProbe prog;

    UniformDefinitionBase world{};
    world.type = UT_MATRIX_4X4;
    world.logicalIndex = 7;
    world.arraySize = 1;

    UniformDefinitionBase tint{};
    tint.type = UT_FLOAT4;
    tint.logicalIndex = 9;
    tint.arraySize = 1;

    prog.SetUniform( "uWorld", world );
    prog.SetUniform( "uTint", tint );

    UniformDefinitionMap out;
    out["should_be_replaced"].type = UT_INT1;

    const bool reflected = rs.ReflectProgram( prog, out );

    EXPECT_TRUE( reflected );
    ASSERT_EQ( 2u, out.size() );
    ASSERT_TRUE( out.find( "uWorld" ) != out.end() );
    ASSERT_TRUE( out.find( "uTint" ) != out.end() );
    EXPECT_EQ( UT_MATRIX_4X4, out["uWorld"].type );
    EXPECT_EQ( 7u, out["uWorld"].logicalIndex );
    EXPECT_EQ( UT_FLOAT4, out["uTint"].type );
    EXPECT_EQ( 9u, out["uTint"].logicalIndex );
  }
}

TEST( GLPluginApiVersionContract, PluginApiVersionIsBumpedTo2 )
{
  EXPECT_EQ( 2, NE_PLUGIN_API_VERSION ) << "SA-442 requires plugin ABI version bump.";
}

TEST( GLPluginApiVersionContract, PluginVersionMismatchPathThrowsRuntimeErrorAtLoadSite )
{
  std::ifstream alphaCpp( "Source/Nebulae/Alpha/Alpha.cpp" );
  ASSERT_TRUE( alphaCpp.is_open() ) << "Expected to inspect plugin load path source.";

  std::string source( ( std::istreambuf_iterator<char>( alphaCpp ) ), std::istreambuf_iterator<char>() );

  EXPECT_NE( std::string::npos, source.find( "throw std::runtime_error" ) )
    << "Plugin mismatch path should throw runtime_error.";
  EXPECT_EQ( std::string::npos, source.find( "catch ( std::exception&" ) )
    << "Plugin mismatch should propagate runtime_error to caller instead of swallowing it.";
}

} // namespace
