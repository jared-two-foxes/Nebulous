
#include "ExampleApplication.h"
#include <Nebulae/Beta/StateStack/StateStack.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  Nebulae::ExampleApplication app;
  app.Init();
  app.Run();
  return 0;
}
