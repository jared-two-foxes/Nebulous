
#include "GuiSampleState.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>

#include <Nebulae/Beta/StateStack/StateStack.h>


using namespace Nebulae;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  // Create the application.
	StateStack app;
  app.Initiate();

#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  Platform::FileSystemPtr fileSystem = app.GetPlatform()->GetFileSystem();
  fileSystem->Mount("disk", new DiskFileDevice("..//..//Samples//Entity//Assets"));
#else
  NE_ASSERT(false, "Zip device is not supported for example.")();
#endif

  app.PushState( new GuiSampleState("GuiSample") );
  app.Run();

  return 0;
  
}
