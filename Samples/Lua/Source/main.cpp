// main.cpp

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>

#include <Nebulae/Beta/StateStack/StateStack.h>

#include "ExampleApplication.h"

using namespace Nebulae;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  Nebulae::ExampleApplication app;
  app.Init();
  app.Run();
  return 0;
}
