#include <Nebulae/Common/Common.h>

#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>
#include <Nebulae/Common/Base/Log/Log.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/MaterialSerializer.h>

#include <Mock/MockRenderSystem.h>
#include <Mock/MockWindow.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>

using namespace Nebulae;

using ::testing::NiceMock;

class CapturingLogSink : public ISink
{
public:
  void Write( const LogRecord& record ) override { records.push_back( record ); }
  void Flush() override {}

  std::vector<LogRecord> records;
};

class MaterialSerializerFixture : public ::testing::Test
{
protected:
  MaterialSerializer* serializer; //< sut
  std::shared_ptr<FileSystem> fileSystem;
  std::shared_ptr<Window> window;
  std::shared_ptr<MockRenderDevice> device;

protected:
  virtual void SetUp()
  {
    fileSystem = std::shared_ptr<FileSystem>( new FileSystem() );
    fileSystem->Mount( "disk", new DiskFileDevice( "../../tests/Assets" ) );

    window = std::shared_ptr<MockWindow>( new MockWindow() );
    device = std::shared_ptr<MockRenderDevice>( new NiceMock<MockRenderDevice>( fileSystem, window ) );
    device->Initiate();

    serializer = new MaterialSerializer( device );
  };

  virtual void TearDown()
  {
    delete serializer;
    serializer = nullptr;
    device.reset();
    device = nullptr;
    fileSystem.reset();
    fileSystem = nullptr;
    window.reset();
    window = nullptr;
  }
};


TEST_F( MaterialSerializerFixture, Serialize_ValidStream_PopulatesMaterial )
{
  // arrange
  Material* material = new Material( "material_01" );

  // act
  File* is = fileSystem->Open( "disk", "materials/debug.material" );
  EXPECT_TRUE( is != NULL );
  bool result = serializer->Serialize( *is, material );

  // assert
  delete material;
  ASSERT_TRUE( result );
}

TEST_F( MaterialSerializerFixture, Serialize_PassLevelUniforms_LogsDeprecationWarning )
{
  Logger logger( "MaterialSerializerTest" );
  auto sink = std::make_shared<CapturingLogSink>();
  logger.AddSink( sink );
  NE_SetModuleLogger( &logger );

  Material material( "pass_level_uniforms" );
  File* stream = fileSystem->Open( "disk", "materials/pass_level_uniforms.material" );
  ASSERT_NE( stream, nullptr );
  ASSERT_TRUE( serializer->Serialize( *stream, &material ) );

  logger.Flush();
  const auto warning = std::find_if( sink->records.begin(), sink->records.end(), []( const LogRecord& record ) {
    return record.message ==
           "Deprecated 'uniforms' block found in material file; uniforms are now auto-reflected from shaders.";
  } );
  EXPECT_NE( warning, sink->records.end() )
    << "A legacy uniforms block at pass level must emit a deprecation warning.";

  NE_SetModuleLogger( nullptr );
}

TEST( MaterialSerializerSourceContract, RemoveDeadUniformTypeConversionHelper )
{
  const std::string relativePath = "Source/Nebulae/Beta/Material/MaterialSerializer.cpp";
  std::string sourcePath = relativePath;

  // Bazel may execute tests from an execroot where source files are available
  // only through the runfiles tree (or its manifest).
  const char* runfilesDir = std::getenv( "RUNFILES_DIR" );
  if ( !runfilesDir )
    runfilesDir = std::getenv( "TEST_SRCDIR" );
  if ( runfilesDir )
    sourcePath = std::string( runfilesDir ) + "/" + relativePath;

  std::ifstream serializerCpp( sourcePath, std::ios::binary );
  if ( !serializerCpp.is_open() )
  {
    const char* manifestPath = std::getenv( "RUNFILES_MANIFEST_FILE" );
    if ( manifestPath )
    {
      std::ifstream manifest( manifestPath );
      std::string logicalPath;
      std::string realPath;
      while ( std::getline( manifest, logicalPath ) )
      {
        const std::string::size_type separator = logicalPath.find( ' ' );
        if ( separator == std::string::npos )
          continue;
        realPath = logicalPath.substr( separator + 1 );
        logicalPath.resize( separator );
        if ( logicalPath.size() >= relativePath.size() &&
             logicalPath.compare( logicalPath.size() - relativePath.size(), relativePath.size(), relativePath ) == 0 )
        {
          serializerCpp.clear();
          serializerCpp.open( realPath, std::ios::binary );
          break;
        }
      }
    }
  }

  ASSERT_TRUE( serializerCpp.is_open() ) << "Expected to inspect MaterialSerializer source.";

  const std::string source( ( std::istreambuf_iterator<char>( serializerCpp ) ), std::istreambuf_iterator<char>() );
  EXPECT_EQ( std::string::npos, source.find( "ConvertStringToUniformType" ) )
    << "MaterialSerializer should not retain the dead uniform type conversion helper.";
}

TEST( MaterialSerializerSourceContract, DoesNotIncludeLegacyUniformDefinitionHeader )
{
  const std::string relativePath = "Source/Nebulae/Beta/Material/MaterialSerializer.cpp";
  std::string sourcePath = relativePath;

  const char* runfilesDir = std::getenv( "RUNFILES_DIR" );
  if ( !runfilesDir )
    runfilesDir = std::getenv( "TEST_SRCDIR" );
  if ( runfilesDir )
    sourcePath = std::string( runfilesDir ) + "/" + relativePath;

  std::ifstream serializerCpp( sourcePath, std::ios::binary );
  if ( !serializerCpp.is_open() )
  {
    const char* manifestPath = std::getenv( "RUNFILES_MANIFEST_FILE" );
    if ( manifestPath )
    {
      std::ifstream manifest( manifestPath );
      std::string logicalPath;
      std::string realPath;
      while ( std::getline( manifest, logicalPath ) )
      {
        const std::string::size_type separator = logicalPath.find( ' ' );
        if ( separator == std::string::npos )
          continue;
        realPath = logicalPath.substr( separator + 1 );
        logicalPath.resize( separator );
        if ( logicalPath.size() >= relativePath.size() &&
             logicalPath.compare( logicalPath.size() - relativePath.size(), relativePath.size(), relativePath ) == 0 )
        {
          serializerCpp.clear();
          serializerCpp.open( realPath, std::ios::binary );
          break;
        }
      }
    }
  }

  ASSERT_TRUE( serializerCpp.is_open() ) << "Expected to inspect MaterialSerializer source.";

  const std::string source( ( std::istreambuf_iterator<char>( serializerCpp ) ), std::istreambuf_iterator<char>() );
  EXPECT_EQ( std::string::npos, source.find( "#include <Nebulae/Alpha/Shaders/UniformDefinition.h>" ) )
    << "MaterialSerializer should not include the unused UniformDefinition header.";
}
