
#include "ExampleScreen.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/StateStack.h>


using namespace Nebulae;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  StateStack app;
  app.Initiate("../../Samples/Application/Assets");
  app.PushState( new ExampleScreen("AudioTest", app.GetRenderSystem()) );
  app.Run();
  return 0;
}