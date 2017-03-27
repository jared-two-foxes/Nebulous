
#include "GuiSampleState.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/StateStack/StateStack.h>

#if NE_PLATFORM == NE_PLATFORM_WIN32
//#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
//#elif NE_PLATFORM == NE_PLATFORM_APPLE
//#include "OSX/AppDelegate.h"
#elif NE_PLATFORM == NE_PLATFORM_APPLE_IOS
#include "iOS/AppDelegate.h"
#endif

using namespace Nebulae;

#ifdef _WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
#else
int main( int argc, char* argv[] )
#endif
{

#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, @"UIApplication", NSStringFromClass([AppDelegate class]));
	[pool release];
	return retVal;
// #elif (NE_PLATFORM == NE_PLATFORM_APPLE) && __LP64__
// 	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
//    
//  mAppDelegate = [[AppDelegate alloc] init];
//  [[NSApplication sharedApplication] setDelegate:mAppDelegate];
// 	int retVal = NSApplicationMain(argc, (const char **) argv);
//
// 	[pool release];
//
// 	return retVal;
#else

  // Create the application.
	StateStack app;
  app.Initiate();

  Platform::FileSystemPtr fileSystem = app.GetPlatform()->GetFileSystem();

#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  fileSystem->Mount( "disk", new DiskFileDevice("..//..//Samples//Particle//Assets") );
#else
  NE_ASSERT( false, "Zip device is not supported for example." )();
#endif

  app.PushState( new GuiSampleState("GuiSample") );

  app.Run();

#endif
  
  return 0;
  
}
