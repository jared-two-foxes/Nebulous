
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Gui/GuiManager.h>


#include <Mock/MockRenderSystem.h>
#include <Mock/MockWindow.h>
#include <Mock/MockWidget.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(Widget, ConstructorCanSetDimensions) 
{
  // arrange
  const Point upperLeft  = Point(0,0);
  const Point lowerRight = Point(20,20);
  std::shared_ptr<FileSystem >        fileSystem;
  std::shared_ptr<MockWindow >        window( new MockWindow() );
  std::shared_ptr<MockRenderDevice >  device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );

  //act
  MockWidget* widget = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);  

  //assert
  ASSERT_LE( upperLeft, widget->GetUpperLeft() );
  ASSERT_GE( lowerRight, widget->GetLowerRight() );
}


TEST(Widget, AttachChildShouldAddValidChildToChildrenList) 
{
  // arrange
  const Point upperLeft  = Point(0,0);
  const Point lowerRight = Point(20,20);
  std::shared_ptr<FileSystem >        fileSystem;
  std::shared_ptr<MockWindow >        window( new MockWindow( ) );
  std::shared_ptr<MockRenderDevice >  device( new MockRenderDevice( fileSystem, window ) );
  GuiManager mgr( fileSystem, device );

  MockWidget* parent = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
  MockWidget* child  = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);

  //act
  parent->AttachChild( child );

  //assert
  const std::list<Widget*>& children = parent->GetChildren();
  ASSERT_TRUE( std::find(children.begin(), children.end(), child) != children.end() );
}


TEST(Widget, AttachChildShouldSetParentOfWithValidChild) 
{
  // arrange
  const Point upperLeft  = Point(0,0);
  const Point lowerRight = Point(20,20);
  std::shared_ptr<FileSystem >        fileSystem;
  std::shared_ptr<MockWindow >        window( new MockWindow( ) );
  std::shared_ptr<MockRenderDevice >  device( new MockRenderDevice( fileSystem, window ) );
  GuiManager mgr( fileSystem, device );

  MockWidget* parent = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
  MockWidget* child  = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);

  //act
  parent->AttachChild( child );

  //assert
  ASSERT_TRUE( child->GetParent() == parent );
}

TEST(Widget, AttachChildShouldReparentChildIfAlreadyParented) 
{
  // arrange
  const Point upperLeft  = Point(0,0);
  const Point lowerRight = Point(20,20);
  std::shared_ptr<FileSystem >        fileSystem;
  std::shared_ptr<MockWindow >        window( new MockWindow( ) );
  std::shared_ptr<MockRenderDevice >  device( new MockRenderDevice( fileSystem, window ) );
  GuiManager mgr( fileSystem, device );

  MockWidget* original = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
  MockWidget* parent   = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
  MockWidget* child    = new MockWidget(*mgr.GetWidgetFactory(), upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
  original->AttachChild( child );

  //act
  parent->AttachChild( child );

  //assert
  EXPECT_TRUE( child->GetParent() == parent );

  const std::list<Widget*>& children = original->GetChildren();
  ASSERT_TRUE( std::find(children.begin(), children.end(), child) == children.end() );
}
