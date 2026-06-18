
#include "ExampleScreen.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/StateStack.h>

using namespace Nebulae;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  Logger logger( "Application", Level::Trace );
  logger.AddSink( std::make_shared<FileSink>( "nebulous_debug.log" ), Level::Trace );
  logger.AddSink( std::make_shared<ConsoleSink>(), Level::Warning );
  NE_SetModuleLogger( &logger );
  NE_LOG_TRACE( "Application", "Debug logging initialized. Logging to nebulous_debug.log" );

  StateStack app;
  app.Initiate( 800, 600 );
  app.PushState( new ExampleScreen( "Application", app.GetRenderSystem() ) );
  app.Run();
  return 0;
}
