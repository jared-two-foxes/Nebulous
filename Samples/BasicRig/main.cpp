//
// The following code will create a VERY basic application using the lowest level classes possible as an example on what is least amont
// of work required to get an application working (no rendering etc), just a working application using the lowest level Platform classes 
// available through the Nebulae Libraries.
//

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/Window/Win32/ClassRegisterationUtility.h>
#include <Nebulae/Common/Window/Win32/Win32Utils.h>
#include <Nebulae/Common/Window/Win32/Win32Window.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>


using namespace Nebulae;


//NebulaeWndProc
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if( uMsg == WM_CREATE )
  { 
    // Grab pointer to the Win32Window
    LPCREATESTRUCT pCreateStruct = (LPCREATESTRUCT)lParam;
    Window* pWindow = (Window*)pCreateStruct->lpCreateParams;

    // Store pointer to Win32Window in user data area
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWindow);

    return 0;
  }

  // look up window instance
  // note: it is possible to get a WM_SIZE before WM_CREATE
  Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  if( !pWindow )
  {
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  switch( uMsg )
  {
  case WM_GETMINMAXINFO:
    // Prevent the window from going smaller than some minimum size
    ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
    ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
    break;
  case WM_CLOSE:
    pWindow->Destroy();
    //PostQuitMessage( 0 );
    return 0;
    break;

  case WM_PAINT:
  	{
  		PAINTSTRUCT ps;
  		HDC hDC = ::BeginPaint( hWnd, &ps );
  		::EndPaint( hWnd, &ps );
  	}
    break;

  default:
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
  }

  return 1;
}

int 
WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow )
{
  // Setup engine required platforming systems.
  Win32Utils::InitiateOS();

  ClassRegisterationUtility registrationUtility;
  registrationUtility.Register( "NebulaeWindowClass", WndProc, hInstance, NULL );

  Platform::WindowPtr pRenderWindow = std::make_shared<Win32Window >( "NebulaeWindowClass", nullptr );
  pRenderWindow->MoveAndResize( 0, 0, 960, 640 );
  pRenderWindow->SetCaption( "render" );
  pRenderWindow->Initiate( nullptr );
  pRenderWindow->Show();

  Platform::FileSystemPtr pFileSystem = std::make_shared<FileSystem >();
  pFileSystem->Mount( "disk", new Nebulae::DiskFileDevice("") );

  std::shared_ptr<RenderSystem > pRenderSystem = CreateRenderSystem( OPENGL_3, pFileSystem, pRenderWindow );
  pRenderSystem->Initiate();

  pRenderSystem->SetClearColour( 1.0f,0.0f,0.0f,0.0f );

  // Loop and update?
  bool bExit = false;
  while ( !bExit )
  {
    // This is the basic platform message pump.  If it receives a Quit message from the system it will return a non zero value.
    bExit = (Win32Utils::MessagePump() != 0);

    pRenderSystem->Clear();

    pRenderSystem->SwapBuffers();
  }

  Win32Utils::TeardownOS();

  return 0;
}