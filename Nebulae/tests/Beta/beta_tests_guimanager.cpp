
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>

#include <Mock/MockWindow.h>
#include <Mock/MockRenderSystem.h>
#include <Mock/MockWidget.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace Nebulae;


TEST(MatchesOrContains, ShouldMatchWhenPassedAMatch) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  
  GuiManager mgr( fileSystem, device );
  Widget*    widget = new MockWidget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );

  //act
  bool result = MatchesOrContains( widget, widget );

  //assert
  
  ASSERT_TRUE( result );
}


TEST(MatchesOrContains, ShouldFailOnDifferentWidgets) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );
  Widget*    first  = new MockWidget(*mgr.GetWidgetFactory(), 0, 0, 20, 20);
  Widget*    second = new MockWidget(*mgr.GetWidgetFactory(), 0, 0, 20, 20);

  //act
  bool result = MatchesOrContains( first, second );

  //assert
  ASSERT_FALSE( result );
}


TEST(MatchesOrContains, ShouldSucceedWhenPassedAChild) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );
  MockWidget first( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  MockWidget second( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );

  //act
  first.AttachChild( &second );
  bool result = MatchesOrContains( &first, &second );

  //assert
  ASSERT_TRUE( result );
}


TEST(MatchesOrContains, ShouldFailWhenPassedAParent) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );
  MockWidget first( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  MockWidget second( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );

  //act
  first.AttachChild( &second );
  bool result = MatchesOrContains( &second, &first );

  //assert
  ASSERT_FALSE( result );
}


TEST(GuiManager, RegisterShouldIgnoreNull) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );

  //act
  mgr.Register( NULL );

  //assert
  const std::list<Widget*>& widgets = mgr.GetRegisteredWidgets();
  ASSERT_TRUE( std::find(widgets.begin(), widgets.end(), static_cast<Widget*>(nullptr)) == widgets.end() );
}


TEST(GuiManager, RegisterShouldAddValidWidgetToDepthList) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );
  MockWidget widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );

  //act
  mgr.Register( &widget );

  //assert
  const std::list<Widget*>& widgets = mgr.GetRegisteredWidgets();
  ASSERT_TRUE( std::find(widgets.begin(), widgets.end(), &widget) != widgets.end() );
}


TEST(GuiManager, RemoveShouldEraseWidgetFromDepthList) {
  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager mgr( fileSystem, device );
  MockWidget widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  mgr.Register( &widget );

  //act
  const std::list<Widget*>& preRemovalWidgets = mgr.GetRegisteredWidgets();
  EXPECT_TRUE( std::find(preRemovalWidgets.begin(), preRemovalWidgets.end(), &widget) != preRemovalWidgets.end() );
  mgr.Remove( &widget );

  //assert
  const std::list<Widget*>& postRemovalWidgets = mgr.GetRegisteredWidgets();
  ASSERT_TRUE( std::find(postRemovalWidgets.begin(), postRemovalWidgets.end(), &widget) == postRemovalWidgets.end() );
}

TEST(GuiManager, HandleEvent_TouchedWithinWidgetBounds_RaisesTouchDownOnWidget) 
{
  using ::testing::_;
  using ::testing::NiceMock;

  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager           mgr( fileSystem, device );
  NiceMock<MockWidget> widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  mgr.Register( &widget );

  EXPECT_CALL( widget, TouchDown(_,_) )
    .Times(1);

  //act
  mgr.HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(10,10), Point(0,0) );
}


TEST(GuiManager, HandleEvent_TouchDraggedWithinWidgetBounds_RaisesTouchDragOnWidget) 
{
  using ::testing::_;
  using ::testing::NiceMock;

  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager           mgr( fileSystem, device );
  NiceMock<MockWidget> widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  mgr.Register( &widget );

  EXPECT_CALL( widget, TouchDrag(_,_,_) )
    .Times(1);

  //act
  mgr.HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(10,10), Point(0,0) );
  boost::this_thread::sleep_for( boost::chrono::milliseconds(255) );
  mgr.HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(15,15), Point(0,0) );
}


/*
TEST(GuiManager, HandleEventRaisesTouchEnterOnWidgetWhenTouchDraggedIntoBounds) {
TEST(GuiManager, HandleEventRaisesTouchHereOnWidgetWhenTouchDraggedThroughBounds) {
TEST(GuiManager, HandleEventRaisesTouchExitOnWidgetWhenTouchDraggedOutOfBounds) {
  //arrange
  GuiManager  mgr;
  MockWidget* widget = new MockWidget(*mgr.GetWidgetFactory(), 5, 5, 25, 25 );
  mgr.Register( widget );

  //act
  mgr.HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, Point(0,0), Point(0,0) );
  boost::this_thread::sleep_for( boost::chrono::milliseconds(255) );
  mgr.HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, Point(10,10), Point(0,0) );
  boost::this_thread::sleep_for( boost::chrono::milliseconds(255) );
  mgr.HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, Point(15,15), Point(0,0) );
  boost::this_thread::sleep_for( boost::chrono::milliseconds(255) );
  mgr.HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, Point(20,20), Point(0,0) );
  boost::this_thread::sleep_for( boost::chrono::milliseconds(255) );
  mgr.HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN, Point(30,30), Point(0,0) );

  bool result = widget->WasTouchDragRecieved();
  ASSERT_TRUE( result );
}*/


TEST(GuiManager, HandleEvent_TouchIsReleasedInWidgetBounds_RaisesClickedOnWidget)
{
  using ::testing::_;
  using ::testing::NiceMock;

  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager           mgr( fileSystem, device );
  NiceMock<MockWidget> widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  mgr.Register( &widget );

  EXPECT_CALL( widget, Clicked(_,_) )
    .Times(1);

  //act
  mgr.HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(10,10), Point(0,0) );
  mgr.HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(10,10), Point(0,0) );
}


TEST(GuiManager, HandleEvent_TouchIsReleasedOutOfWidgetBounds_RaisesTouchUpOnWidget) 
{
  using ::testing::_;
  using ::testing::NiceMock;

  // arrange
  std::shared_ptr<FileSystem>        fileSystem( new FileSystem() );
  std::shared_ptr<MockWindow >       window( new MockWindow() );
  std::shared_ptr<MockRenderDevice > device( new MockRenderDevice(fileSystem, window) );
  GuiManager           mgr( fileSystem, device );
  NiceMock<MockWidget> widget( *mgr.GetWidgetFactory(), 0, 0, 20, 20 );
  mgr.Register( &widget );

  EXPECT_CALL( widget, TouchUp(_,_) )
    .Times(1);

  //act
  mgr.HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(10,10), Point(0,0) );
  mgr.HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN, 0, MOD_KEY_NONE, Point(25,25), Point(0,0) );
}