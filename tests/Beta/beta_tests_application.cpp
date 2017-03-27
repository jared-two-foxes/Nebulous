
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>
#include <Nebulae/Beta/Application/Application.h>

#include "gtest/gtest.h"

using namespace Nebulae;


class MockInputListener : public InputListener {
public:
  virtual void KeyPressed( KeyCode keyCode, Flags<ModKey> modKeys ) {}
  virtual void KeyReleased( KeyCode keyCode, Flags<ModKey> modKeys ) {}

  virtual void MousePressed( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}
  virtual void MouseReleased( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}
  virtual void MouseMove( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}
};


TEST(Application, InitiateShouldSucceedWithCorrectValues) {
  // arrange
  Application app;
  
  //act
  app.Initiate( 800, 600 );

  //assert
  ASSERT_TRUE( app.IsInitialized() );
}


TEST(Application, InitiateShouldSucceedWithDefaultValues) {
  // arrange
  Application app;
  
  //act
  app.Initiate();

  //assert
  ASSERT_TRUE( app.IsInitialized() );
}


TEST(Application, HasInputListenerShouldSucceedWithValidListener) {
  // arrange
  Application app;
  app.Initiate();
  MockInputListener* listener = new MockInputListener();
  app.AddInputListener( listener );

  //act
  bool result = app.HasInputListener( listener );

  //assert
  ASSERT_TRUE( result );

  delete listener;
}


TEST(Application, HasInputListenerShouldFailWithAnInvalidListener) {
  // arrange
  Application app;
  app.Initiate();

  //act
  bool result = app.HasInputListener( NULL );

  //assert
  ASSERT_FALSE( result );
}


TEST(Application, AddInputListenerShouldSucceedWithValidListener) {
  // arrange
  Application app;
  app.Initiate();
  MockInputListener* listener = new MockInputListener();

  //act
  app.AddInputListener( listener );
  bool result = app.HasInputListener(listener);

  //assert
  ASSERT_TRUE( result );

  delete listener;
}