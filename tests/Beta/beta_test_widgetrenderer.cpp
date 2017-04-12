
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/WidgetRenderer.h>
#include <Nebulae/Beta/Material/Material.h>

#include <Mock/MockWindow.h>
#include <Mock/MockRenderSystem.h>
#include <Mock/MockWidget.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace Nebulae;

using ::testing::NiceMock;

class WidgetRendererFixture : public ::testing::Test
{
  
  protected:
    std::shared_ptr<FileSystem >                   fileSystem;
    std::shared_ptr<Window >                       window;
    std::shared_ptr<NiceMock<MockRenderDevice > >  device;
    WidgetRenderer*                                renderer;

  protected:
    virtual void SetUp() 
    {
      fileSystem = std::make_shared<FileSystem >();
      fileSystem->Mount( "disk", new DiskFileDevice("../../tests/Assets") );
      window     = std::make_shared<MockWindow >();
      
      device     = std::make_shared<NiceMock<MockRenderDevice > >( fileSystem, window );

      device->Initiate();
      
      renderer = new WidgetRenderer( fileSystem, nullptr, device );
    };

    virtual void TearDown() 
    {
      delete renderer; renderer = nullptr;
      device.reset(); device = nullptr;
      fileSystem.reset(); fileSystem = nullptr;
      window.reset(); window = nullptr;
    }

};

TEST_F(WidgetRendererFixture, Render_DrawsButtons) 
{
  using ::testing::_;

  ::testing::DefaultValue<UniformDefinition>::Set( UniformDefinition() );

  //arrange
  renderer->Init();

  GuiManager         mgr( fileSystem, device );
  std::list<Widget*> widgetList;
  widgetList.push_back( new ButtonControl(*mgr.GetWidgetFactory(), 0, 0, 1, 1, "caption", std::shared_ptr<Font>(), CLR_BLACK, CLR_WHITE ) );

  EXPECT_CALL( *device, Draw(_,_) )
      .Times(5);

  //act
  renderer->Render( widgetList );
}
