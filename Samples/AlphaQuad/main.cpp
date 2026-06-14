#include "quad_resources.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Common/FileSystem/ManifestDiskFileDevice.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>
#include <Nebulae/Common/Window/Window.h>

using namespace Nebulae;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd )
{
  (void)hInstance;
  (void)lpCmdLine;
  (void)nShowCmd;

  // Create the platform.
  std::shared_ptr<Platform> platform = CreateAndInitiatePlatform();
  if ( !platform || !platform->Initiate() )
  {
    return 1;
  }

  // Mount the disk filesystem.
  FileSystem* fileSystem = platform->GetFileSystem().get();
  fileSystem->Mount( "disk", new ManifestDiskFileDevice( "" ) );

  // Create a window.
  std::shared_ptr<Window> window = platform->CreateApplicationWindow( 0, 0, 800, 600 );
  if ( !window )
  {
    return 1;
  }
  window->Show();

  // Create the OpenGL 3 render system.
  std::shared_ptr<RenderSystem> renderSystem = CreateRenderSystem( OPENGL_3, platform->GetFileSystem(), window );
  if ( !renderSystem || !renderSystem->Initiate() )
  {
    return 1;
  }

  // Set operation type.
  renderSystem->SetOperationType( OT_TRIANGLES );

  // Initialize resources.
  QuadResources resources = {};
  if ( !InitResources( renderSystem.get(), resources ) )
  {
    return 1;
  }

  // Build the render stream.
  RenderStream stream = BuildQuadStream( resources );

  // Main loop.
  RenderFrame( platform.get(), renderSystem.get(), stream );

  return 0;
}
