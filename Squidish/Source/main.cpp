
#include "ExampleApplication.h"
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/StateStack/StateStack.h>


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  Game::ExampleApplication app;
  app.Init();
  app.Run();
  return 0;
}
