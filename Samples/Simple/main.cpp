
#include "SimpleState.h"

#include <Nebulae/Common/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/StateStack/StateStack.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  // Create the application.
  Nebulae::StateStack app;
  app.Initiate( 800, 600 );
  app.PushState( new Sample::SimpleState("Simple") );
  app.Run();
  
  return 0;
}
