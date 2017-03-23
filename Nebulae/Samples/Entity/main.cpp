// main.cpp

#include "States/SampleState.h"

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
using namespace Sample;

#ifdef _WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
#else
int main( int argc, char* argv[] )
#endif
{
  int retVal = 0;

#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	retVal = UIApplicationMain(argc, argv, @"UIApplication", NSStringFromClass([AppDelegate class]));
	[pool release];
	return retVal;
 #elif (NE_PLATFORM == NE_PLATFORM_APPLE) && __LP64__
 	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
  mAppDelegate = [[AppDelegate alloc] init];
  [[NSApplication sharedApplication] setDelegate:mAppDelegate];
 	retVal = NSApplicationMain(argc, (const char **) argv);

 	[pool release];

 	return retVal;
#else
	StateStack app;
  app.Initiate();

#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  Platform::FileSystemPtr fileSystem = app.GetPlatform()->GetFileSystem();
  fileSystem->Mount( "disk", new DiskFileDevice("..//..//Samples//Entity//Assets") );
#else
  NE_ASSERT( false, "Zip device is not supported for example." )();
#endif

  app.PushState( new SampleState() );
  app.Run();
#endif

  return retVal;
}