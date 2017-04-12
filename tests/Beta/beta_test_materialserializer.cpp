
#include <Nebulae/Common/Common.h>

#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/MaterialSerializer.h>

#include <Mock/MockWindow.h>
#include <Mock/MockRenderSystem.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace Nebulae;

using ::testing::NiceMock;

class MaterialSerializerFixture : public ::testing::Test 
{
  protected:
    MaterialSerializer*                 serializer; //< sut
    std::shared_ptr<FileSystem >        fileSystem;
    std::shared_ptr<Window >            window;
    std::shared_ptr<MockRenderDevice >  device;

  protected:
    virtual void SetUp() 
    {
      fileSystem = std::shared_ptr<FileSystem >( new FileSystem() );
      fileSystem->Mount( "disk", new DiskFileDevice("../../tests/Assets") );

      window = std::shared_ptr<MockWindow >( new MockWindow() );
      device = std::shared_ptr<MockRenderDevice >( new NiceMock<MockRenderDevice>(fileSystem, window) );
      device->Initiate();

      serializer = new MaterialSerializer( device );
    };

    virtual void TearDown() 
    {
      delete serializer; serializer = nullptr;
      device.reset(); device = nullptr;
      fileSystem.reset(); fileSystem = nullptr;
      window.reset(); window = nullptr;
    }

};


TEST_F(MaterialSerializerFixture, Serialize_ValidStream_PopulatesMaterial) 
{
  // arrange
  Material* material = new Material( "material_01" );

  //act
  File* is = fileSystem->Open( "disk", "materials/debug.material" );
  EXPECT_TRUE( is != NULL );
  bool result = serializer->Serialize( *is, material );
  
  //assert
  delete material;
  ASSERT_TRUE( result );
}
