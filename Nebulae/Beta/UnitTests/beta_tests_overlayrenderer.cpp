
#include <Nebulae/Alpha/Shared/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/Shared/InputLayout/InputLayout.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Overlay/OVerlayRenderer.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include "gtest/gtest.h"

using namespace Nebulae;


class OverlayRendererFixture : public ::testing::Test {
  
  protected:
    std::shared_ptr<Platform >          platform;
    std::shared_ptr<Window >            window;
    std::shared_ptr<RenderSystem >      device;
    OverlayRenderer*                    renderer;

  protected:
    virtual void SetUp() 
    {
      platform   = std::shared_ptr<Platform >( new Win32Platform() );
      platform->Initiate();

      window     = platform->CreateApplicationWindow(0,0,800,600);
      device     = platform->CreateRenderSystem( window );
      device->Initiate();
      
      renderer = new OverlayRenderer( device );
    };

    virtual void TearDown() 
    {
      delete renderer; renderer = nullptr;
      device.reset(); device = nullptr;
      platform->DestroyWindow( window ); 
      window.reset(); window = nullptr;
      platform->Destroy(); platform = nullptr;
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
  OverlayRenderer renderer( nullptr );
  
  //act
  bool result = renderer.Init();  
  
  //assert
  ASSERT_TRUE( !result );
}


TEST(OverlayRenderer, Init_UninitializedRenderDevice_ShouldReturnFalse) 
{
  //arrange
  std::shared_ptr<Platform >     platform   = std::shared_ptr<Platform >( new Win32Platform() );
  platform->Initiate();

  std::shared_ptr<Window >       window     = platform->CreateApplicationWindow( 0, 0, 800, 600 );
  std::shared_ptr<RenderSystem > device     = std::shared_ptr<RenderSystem >( CreateRenderSystem(platform->GetFileSystem(), window) ); //< manually create rendersystem so that it does not get initiated/

  OverlayRenderer renderer( device );

  //act
  bool result = renderer.Init();  

  //assert
  ASSERT_TRUE( !result );
}