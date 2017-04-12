
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Overlay/OverlayRenderer.h>

#include <Mock/MockWindow.h>
#include <Mock/MockRenderSystem.h>
#include <Mock/MockWidget.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace Nebulae;

using ::testing::NiceMock;


class OverlayRendererFixture : public ::testing::Test {
  
  protected:
    std::shared_ptr<FileSystem >        fileSystem;
    std::shared_ptr<Window >            window;
    std::shared_ptr<RenderSystem >      device;
    OverlayRenderer*                    renderer;

  protected:
    virtual void SetUp() 
    {
			fileSystem = std::shared_ptr<FileSystem >( new FileSystem() );
			fileSystem->Mount( "disk", new DiskFileDevice("../../tests/Assets") );
			window     = std::shared_ptr<MockWindow >( new MockWindow() );
			device     = std::shared_ptr<MockRenderDevice >( new NiceMock<MockRenderDevice>(fileSystem, window) );
      
			device->Initiate();
			SpriteBatch* batcher = nullptr;
      
			renderer = new OverlayRenderer( device, batcher );
    };

    virtual void TearDown() 
    {
      delete renderer; renderer = nullptr;
      device.reset(); device = nullptr; 
      window.reset(); window = nullptr;
      fileSystem._Reset(); fileSystem = nullptr;
    }

};


TEST_F(OverlayRendererFixture, Init_ValidDevice_ShouldSucceed) 
{
  //act
  bool result = renderer->Init();  

  //assert
  ASSERT_TRUE( result );
}


TEST_F(OverlayRendererFixture, Init_ShouldSetupBuffer) 
{
  const std::string bufferName = "GuiVertexBuffer";

  //act
  bool result = renderer->Init();  
  EXPECT_TRUE( result );
  HardwareBuffer* buffer = device->FindBufferByName( bufferName.c_str() );

  //assert
  ASSERT_TRUE( buffer != NULL );
  //ASSERT_EQ( HBT_VERTEX, buffer->GetType() );
}


TEST_F(OverlayRendererFixture, Init_ShouldSetupInputLayout) 
{
  const std::string layoutName = "BasicGuiLayout";

  //act
  bool result = renderer->Init();  
  EXPECT_TRUE( result );
  InputLayout*   layout = device->FindInputLayoutByName( layoutName.c_str() );

  //assert
  ASSERT_TRUE( layout != NULL );
}


TEST(OverlayRenderer, Init_NullRenderDevice_ShouldReturnFalse) 
{
  //arrange
  OverlayRenderer renderer( nullptr, nullptr );
  
  //act
  bool result = renderer.Init();  
  
  //assert
  ASSERT_TRUE( !result );
}


TEST(OverlayRenderer, Init_UninitializedRenderDevice_ShouldReturnFalse) 
{
  //arrange
  std::shared_ptr<Platform >     platform   = CreateAndInitiatePlatform();
  std::shared_ptr<Window >       window     = platform->CreateApplicationWindow( 0, 0, 800, 600 );
  std::shared_ptr<RenderSystem > device     = std::shared_ptr<RenderSystem >( CreateRenderSystem( OPENGL_3, platform->GetFileSystem(), window ) ); //< manually create rendersystem so that it does not get initiated/

	SpriteBatch* batcher = nullptr;
  OverlayRenderer renderer( device, batcher );

  //act
  bool result = renderer.Init();  

  //assert
  ASSERT_TRUE( !result );
}