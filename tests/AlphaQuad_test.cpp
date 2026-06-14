#include "gtest/gtest.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <string>

namespace
{
std::optional<std::string> ReadTextFile( const std::filesystem::path& path )
{
  std::ifstream in( path, std::ios::binary );
  if ( !in )
    return std::nullopt;

  return std::string( std::istreambuf_iterator<char>( in ), std::istreambuf_iterator<char>() );
}

std::filesystem::path DetectWorkspaceFromManifest()
{
  const char* manifest = std::getenv( "RUNFILES_MANIFEST_FILE" );
  if ( !manifest )
    return {};

  const char* workspace = std::getenv( "TEST_WORKSPACE" );
  const std::string workspaceName = workspace ? workspace : "_main";
  const std::string marker = workspaceName + "/tests/Assets/";

  std::ifstream mf( manifest );
  std::string line;
  while ( std::getline( mf, line ) )
  {
    const std::size_t split = line.find( ' ' );
    if ( split == std::string::npos )
      continue;

    const std::string logical = line.substr( 0, split );
    if ( logical.find( marker ) == std::string::npos )
      continue;

    const std::filesystem::path realPath = line.substr( split + 1 );
    if ( !realPath.has_parent_path() )
      continue;

    // .../tests/Assets/<file>
    return realPath.parent_path().parent_path().parent_path();
  }

  return {};
}

std::filesystem::path WorkspaceRoot()
{
  if ( const char* envRoot = std::getenv( "BUILD_WORKSPACE_DIRECTORY" ); envRoot != nullptr )
  {
    const std::filesystem::path root( envRoot );
    if ( std::filesystem::exists( root / "MODULE.bazel" ) )
      return root;
  }

  // Local non-Bazel fallback.
  for ( std::filesystem::path p = std::filesystem::current_path(); !p.empty(); p = p.parent_path() )
  {
    if ( std::filesystem::exists( p / "MODULE.bazel" ) )
      return p;

    if ( p == p.parent_path() )
      break;
  }

  return DetectWorkspaceFromManifest();
}

void ExpectRegex( const std::string& content, const std::string& pattern )
{
  EXPECT_TRUE( std::regex_search( content, std::regex( pattern ) ) ) << "Missing pattern: " << pattern;
}
} // namespace

TEST( AlphaQuadSample, DirectoryStructureExists )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  const std::filesystem::path sampleDir = root / "Samples" / "AlphaQuad";
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "BUILD.bazel" ) );
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "main.cpp" ) );
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "quad_resources.h" ) );
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "quad_resources.cpp" ) );
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "textured_quad_vs.glsl" ) );
  EXPECT_TRUE( std::filesystem::exists( sampleDir / "textured_quad_ps.glsl" ) );
}

TEST( AlphaQuadSample, BuildBazelIsCorrect )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  const auto buildFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "BUILD.bazel" );
  ASSERT_TRUE( buildFile.has_value() ) << "Missing Samples/AlphaQuad/BUILD.bazel";

  const std::string& content = *buildFile;
  ExpectRegex( content, "cc_library\\s*\\(" );
  ExpectRegex( content, "cc_binary\\s*\\(" );
  ExpectRegex( content, "name\\s*=\\s*\"alpha_quad\"" );
  ExpectRegex( content, "name\\s*=\\s*\"quad_resources\"" );
  ExpectRegex( content, "srcs\\s*=\\s*\\[[^\\]]*\"main\\.cpp\"" );
  ExpectRegex( content, "data\\s*=\\s*\\[[^\\]]*\\.glsl" );
  ExpectRegex( content, "//RenderSystems/GL:nebulous_gl3" );
  ExpectRegex( content, "deps\\s*=\\s*\\[[^\\]]*//Source/Nebulae:nebulae" );
  ExpectRegex( content, "linkopts\\s*=\\s*\\[[^\\]]*/SUBSYSTEM:WINDOWS" );
  ExpectRegex( content, "linkopts\\s*=\\s*\\[[^\\]]*/ENTRY:WinMainCRTStartup" );
}

TEST( AlphaQuadSample, MainCppHasRequiredStructure )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  const auto mainFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "main.cpp" );
  ASSERT_TRUE( mainFile.has_value() ) << "Missing Samples/AlphaQuad/main.cpp";

  const auto implFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "quad_resources.cpp" );
  ASSERT_TRUE( implFile.has_value() ) << "Missing Samples/AlphaQuad/quad_resources.cpp";

  const std::string& mainContent = *mainFile;
  const std::string& implContent = *implFile;

  // main.cpp checks
  ExpectRegex( mainContent, "Win32Platform\\.h" );
  EXPECT_FALSE( std::regex_search( mainContent, std::regex( "#include\\s*[<\"](?:[^\"\\n]*Beta[^\"\\n]*)[>\"]" ) ) )
    << "main.cpp must not include Beta headers.";

  ExpectRegex( mainContent, "CreateAndInitiatePlatform\\s*\\(" );
  ExpectRegex( mainContent, "Mount\\s*\\([^\\)]*\"disk\"" );
  ExpectRegex( mainContent, "Create[^\\n;]*Window\\s*\\(" );
  ExpectRegex( mainContent, "CreateRenderSystem\\s*\\(\\s*OPENGL_3" );
  ExpectRegex( mainContent, "SetOperationType\\s*\\(\\s*OT_TRIANGLES\\s*\\)" );

  // quad_resources.cpp checks (RenderFrame contains the main loop calls)
  ExpectRegex( implContent, "MessagePump\\s*\\(" );
  ExpectRegex( implContent, "Clear\\s*\\(" );
  ExpectRegex( implContent, "ExecuteStream\\s*\\(" );
  ExpectRegex( implContent, "SwapBuffers\\s*\\(" );

  // quad_resources.cpp checks: single CreateBuffer call with 6 vertices (non-indexed quad)
  ExpectRegex( implContent, "CreateBuffer\\s*\\(" );
  ExpectRegex( implContent, "\\[\\s*6\\s*\\]" );

  ExpectRegex( implContent, "CreateTexture\\s*\\(" );
  ExpectRegex( implContent, "LoadFromMemory\\s*\\(" );

  ExpectRegex( implContent, "PT_SetProgram" );
  ExpectRegex( implContent, "PT_SetGeometry" );
  ExpectRegex( implContent, "PT_SetRenderState" );
  ExpectRegex( implContent, "PT_SetSampler" );
  ExpectRegex( implContent, "PT_Draw" );
  ExpectRegex( implContent, "GetUniformByName[^(]*\\(" );
}

TEST( AlphaQuadSample, ShadersHaveRequiredContent )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  const auto vsFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "textured_quad_vs.glsl" );
  ASSERT_TRUE( vsFile.has_value() ) << "Missing vertex shader file.";

  const auto psFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "textured_quad_ps.glsl" );
  ASSERT_TRUE( psFile.has_value() ) << "Missing pixel shader file.";

  const std::string& vs = *vsFile;
  ExpectRegex( vs, "attribute\\s+[^;]*a_vertex" );
  ExpectRegex( vs, "attribute\\s+[^;]*a_texcoord" );
  ExpectRegex( vs, "varying\\s+[^;]*vTexCoord" );
  ExpectRegex( vs, "gl_Position\\s*=" );

  const std::string& ps = *psFile;
  ExpectRegex( ps, "uniform\\s+sampler2D\\s+s_texture" );
  ExpectRegex( ps, "texture2D\\s*\\(" );
  ExpectRegex( ps, "gl_FragColor\\s*=" );
}

TEST( AlphaQuadSample, InitResourcesHasProperErrorHandling )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  const auto implFile = ReadTextFile( root / "Samples" / "AlphaQuad" / "quad_resources.cpp" );
  ASSERT_TRUE( implFile.has_value() ) << "Missing Samples/AlphaQuad/quad_resources.cpp";

  const std::string& content = *implFile;

  // Verify CreateTexture is called with shouldLoad=false to prevent filesystem
  // load before LoadFromMemory().
  ExpectRegex( content, "CreateTexture\\s*\\(\\s*\"Checkerboard\"\\s*,\\s*false\\s*\\)" );

  // Verify the texture null check does NOT use IsLoaded() (since shouldLoad=false
  // means the texture won't be loaded from file; LoadFromMemory is called separately).
  EXPECT_FALSE( std::regex_search(
    content,
    std::regex(
      "CreateTexture\\s*\\(\\s*\"Checkerboard\".*\\n.*!tmp\\.texture\\s*\\|\\|\\s*!tmp\\.texture->IsLoaded" ) ) )
    << "Texture null check should not use IsLoaded() when shouldLoad=false";

  // Verify LoadFromMemory result is validated.
  ExpectRegex( content, "!tmp\\.texture->LoadFromMemory\\s*\\(" );

  // Verify VertexDeceleration is stack-allocated (no heap allocation).
  EXPECT_FALSE( std::regex_search( content, std::regex( "new\\s+VertexDeceleration" ) ) )
    << "VertexDeceleration must be stack-allocated to avoid memory leak.";
  ExpectRegex( content, "VertexDeceleration\\s+\\w+\\s*\\(" );

  // Verify uniform validity check exists.
  ExpectRegex( content, "uniformDef\\.IsValid\\s*\\(\\s*\\)" );

  // Verify cleanup on failure: temporary struct pattern.
  ExpectRegex( content, "QuadResources\\s+tmp\\s*=" );
  ExpectRegex( content, "res\\s*=\\s*tmp" );
}

TEST( AlphaQuadSample, BazelBuildSucceeds )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root.";

  // Skip nested Bazel invocation if running inside a Bazel sandbox
  // (detected by presence of RUNFILES_MANIFEST_FILE or TEST_TMPDIR environment variables)
  const bool inBazelSandbox =
    ( std::getenv( "RUNFILES_MANIFEST_FILE" ) != nullptr ) || ( std::getenv( "TEST_TMPDIR" ) != nullptr );

  if ( inBazelSandbox )
  {
    // When run via `bazel test`, nested `bazel build` fails due to lock conflict.
    // The other 4 tests already verify the build structure, so we skip this check.
    GTEST_SKIP() << "Skipping nested Bazel invocation (running in Bazel sandbox)";
    return;
  }

  const std::filesystem::path previous = std::filesystem::current_path();
  std::filesystem::current_path( root );

#ifdef _WIN32
  const int exitCode = std::system( "bazel build //Samples/AlphaQuad:alpha_quad >NUL 2>&1" );
#else
  const int exitCode = std::system( "bazel build //Samples/AlphaQuad:alpha_quad >/dev/null 2>&1" );
#endif

  std::filesystem::current_path( previous );
  EXPECT_EQ( 0, exitCode ) << "Expected bazel build //Samples/AlphaQuad:alpha_quad to succeed.";
}
