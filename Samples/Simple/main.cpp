
#include "SimpleState.h"
#include <Nebulae/Beta/StateStack/StateStack.h>

#if NEBULAE_PLATFORM == NEBULAE_PLATFORM_WIN32
  #include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#elif NEBULAE_PLATFORM == NEBULAE_PLATFORM_APPLE
  #include "OSX/AppDelegate.h"
#elif NEBULAE_PLATFORM == NEBULAE_PLATFORM_APPLE_IOS
#ifdef __OBJC__
  #import <UIKit/UIKit.h>
  #include "iOS/AppDelegate.h"
#endif
#endif

#ifdef _WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
#else
int main( int argc, char* argv[] )
#endif
{
#if NEBULAE_PLATFORM == NEBULAE_PLATFORM_APPLE_IOS
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  int retVal = UIApplicationMain(argc, argv, @"UIApplication", NSStringFromClass([AppDelegate class]));
  [pool release];
  return retVal;
#else
  // Create the application.
  Nebulae::StateStack app;
  app.Initiate( "../../Samples/Media", 800, 600 );
  app.PushState( new Sample::SimpleState("Simple") );
  app.Run();
  return 0;
#endif
  
}
