// main.cpp

#include "States/SampleState.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>

#include <Nebulae/Beta/StateStack/StateStack.h>

using namespace Nebulae;
using namespace Sample;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  int retVal = 0;

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

  return retVal;
}