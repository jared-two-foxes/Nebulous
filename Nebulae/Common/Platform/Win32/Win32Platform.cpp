
#include "Win32Platform.h"

#include <Nebulae/Common/FileSystem/FileSystem.h>

#include <Nebulae/Common/Platform/Keyboard.h>
#include <Nebulae/Common/Window/Win32/ClassRegisterationUtility.h>
#include <Nebulae/Common/Window/Win32/Win32Window.h>
#include <Nebulae/Common/Window/Win32/Win32Utils.h>

#include <Nebulae/Beta/Gui/WidgetEvent.h>

#include <direct.h>   //for _chdir
#include <fcntl.h>
#include <stdio.h>
#include <io.h>

using namespace Nebulae;

namespace 
{
  Flags<ModKey> GetModKeys( Keyboard* keyboard )
  {
    Flags<ModKey> retval;
    if( keyboard->IsKeyDown(VKC_LSHIFT) )   retval |= MOD_KEY_LSHIFT;
    if( keyboard->IsKeyDown(VKC_RSHIFT) )   retval |= MOD_KEY_RSHIFT;
    if( keyboard->IsKeyDown(VKC_LCTRL) )    retval |= MOD_KEY_LCTRL;
    if( keyboard->IsKeyDown(VKC_RCTRL) )    retval |= MOD_KEY_RCTRL;
    if( keyboard->IsKeyDown(VKC_LALT) )     retval |= MOD_KEY_LALT;
    if( keyboard->IsKeyDown(VKC_RALT) )     retval |= MOD_KEY_RALT;
    if( keyboard->IsKeyDown(VKC_LSUPER) )   retval |= MOD_KEY_LMETA;
    if( keyboard->IsKeyDown(VKC_RSUPER) )   retval |= MOD_KEY_RMETA;
    if( keyboard->IsKeyDown(VKC_NUMLOCK) )  retval |= MOD_KEY_NUM;
    if( keyboard->IsKeyDown(VKC_CAPSLOCK) ) retval |= MOD_KEY_CAPS;
    return retval;
  }

  KeyCode VirtualKeyFromWindowsKey( unsigned short key, Flags<ModKey> mods )
  {
    KeyCode retval( VKC_UNKNOWN );

    switch( key ) 
    {
      case VK_ESCAPE:             retval = VKC_ESCAPE; break;
      case VK_SUBTRACT:           retval = VKC_MINUS; break;
      case VK_OEM_PLUS:           retval = VKC_EQUALS; break;
      case VK_OEM_MINUS:          retval = VKC_MINUS; break;
      case VK_BACK:               retval = VKC_BACKSPACE; break;
      case VK_TAB:                retval = VKC_TAB; break;
      case VK_OEM_4:              retval = VKC_LEFTBRACKET; break;
      case VK_OEM_6:              retval = VKC_RIGHTBRACKET; break;
      case VK_RETURN:             retval = VKC_RETURN; break;
      case VK_LCONTROL:           retval = VKC_LCTRL; break;
      case VK_OEM_1:              retval = VKC_SEMICOLON; break;
      case VK_OEM_7:              retval = VKC_QUOTE; break;
      case VK_LSHIFT:             retval = VKC_LSHIFT; break;
      case VK_OEM_2:              retval = VKC_SLASH; break;
      case VK_OEM_COMMA:          retval = VKC_COMMA; break;
      case VK_OEM_PERIOD:         retval = VKC_PERIOD; break;
      case VK_OEM_3:              retval = VKC_BACKQUOTE; break;
      case VK_OEM_5:              retval = VKC_BACKSLASH; break;
      case VK_RSHIFT:             retval = VKC_RSHIFT; break;
      case VK_MULTIPLY:           retval = VKC_KP_MULTIPLY; break;
      case VK_LMENU:              retval = VKC_LALT; break;
      case VK_SPACE:              retval = VKC_SPACE; break;
      case VK_CAPITAL:            retval = VKC_CAPSLOCK; break;
      case VK_F1:                 retval = VKC_F1; break;
      case VK_F2:                 retval = VKC_F2; break;
      case VK_F3:                 retval = VKC_F3; break;
      case VK_F4:                 retval = VKC_F4; break;
      case VK_F5:                 retval = VKC_F5; break;
      case VK_F6:                 retval = VKC_F6; break;
      case VK_F7:                 retval = VKC_F7; break;
      case VK_F8:                 retval = VKC_F8; break;
      case VK_F9:                 retval = VKC_F9; break;
      case VK_F10:                retval = VKC_F10; break;
      case VK_NUMLOCK:            retval = VKC_NUMLOCK; break;
      case VK_SCROLL:             retval = VKC_SCROLLOCK; break;
      case VK_NUMPAD7:            retval = VKC_KP7; break;
      case VK_NUMPAD8:            retval = VKC_KP8; break;
      case VK_NUMPAD9:            retval = VKC_KP9; break;
      case VK_NUMPAD4:            retval = VKC_KP4; break;
      case VK_NUMPAD5:            retval = VKC_KP5; break;
      case VK_NUMPAD6:            retval = VKC_KP6; break;
      case VK_ADD:                retval = VKC_KP_PLUS; break;
      case VK_NUMPAD1:            retval = VKC_KP1; break;
      case VK_NUMPAD2:            retval = VKC_KP2; break;
      case VK_NUMPAD3:            retval = VKC_KP3; break;
      case VK_NUMPAD0:            retval = VKC_KP0; break;
      case VK_OEM_102:            retval = VKC_UNKNOWN; break;
      case VK_F11:                retval = VKC_F11; break;
      case VK_F12:                retval = VKC_F12; break;
      case VK_F13:                retval = VKC_F13; break;
      case VK_F14:                retval = VKC_F14; break;
      case VK_F15:                retval = VKC_F15; break;
      case VK_KANA:               retval = VKC_UNKNOWN; break;
      case VK_CONVERT:            retval = VKC_UNKNOWN; break;
      case VK_NONCONVERT:         retval = VKC_UNKNOWN; break;
      case VK_KANJI:              retval = VKC_UNKNOWN; break;
      case VK_RCONTROL:           retval = VKC_RCTRL; break;
      case VK_DIVIDE:             retval = VKC_KP_DIVIDE; break;
      case VK_RMENU:              retval = VKC_RALT; break;
      case VK_PAUSE:              retval = VKC_PAUSE; break;
      case VK_HOME:               retval = VKC_HOME; break;
      case VK_UP:                 retval = VKC_UP; break;
      case VK_LEFT:               retval = VKC_LEFT; break;
      case VK_RIGHT:              retval = VKC_RIGHT; break;
      case VK_END:                retval = VKC_END; break;
      case VK_DOWN:               retval = VKC_DOWN; break;
      case VK_INSERT:             retval = VKC_INSERT; break;
      case VK_DELETE:             retval = VKC_DELETE; break;
      case VK_APPS:               retval = VKC_UNKNOWN; break;
      case VK_SLEEP:              retval = VKC_UNKNOWN; break;
      
      default: retval = KeyCode(key);
    }

    // this code works because Nebulae::KeyCodes maps (at least partially) to the
    // printable ASCII characters
    if( mods & MOD_KEY_SHIFT )
    {
      // this assumes a US keyboard layout
      switch( retval ) 
      {
        case '`':  retval = KeyCode('~'); break;
        case '1':  retval = KeyCode('!'); break;
        case '2':  retval = KeyCode('@'); break;
        case '3':  retval = KeyCode('#'); break;
        case '4':  retval = KeyCode('$'); break;
        case '5':  retval = KeyCode('%'); break;
        case '6':  retval = KeyCode('^'); break;
        case '7':  retval = KeyCode('&'); break;
        case '8':  retval = KeyCode('*'); break;
        case '9':  retval = KeyCode('('); break;
        case '0':  retval = KeyCode(')'); break;
        case '-':  retval = KeyCode('_'); break;
        case '=':  retval = KeyCode('+'); break;
        case '[':  retval = KeyCode('{'); break;
        case ']':  retval = KeyCode('}'); break;
        case '\\': retval = KeyCode('|'); break;
        case ';':  retval = KeyCode(':'); break;
        case '\'': retval = KeyCode('"'); break;
        case ',':  retval = KeyCode('<'); break;
        case '.':  retval = KeyCode('>'); break;
        case '/':  retval = KeyCode('?'); break;
        default: break;
      }
    }

    return retval;
  }
}

LRESULT CALLBACK NebulaeWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if( uMsg == WM_CREATE ) 
  {
    // Grab pointer to the Win32Window from CREATESTRUCT and store it for later access.
    LPCREATESTRUCT pCreateStruct = ( LPCREATESTRUCT )lParam;
    Win32Platform* platform      = ( Win32Platform* ) pCreateStruct->lpCreateParams;
    SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)platform );

    // Init the Input devices that we wish to listen to;  0 - Keyboard, 1 - Mouse.
    RAWINPUTDEVICE rid[2];
    rid[0].usUsagePage = 1; 
    rid[0].usUsage = 6;
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = NULL;
    rid[1].usUsagePage = 1;
    rid[1].usUsage = 2;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = NULL;
    RegisterRawInputDevices( rid, 2, sizeof(RAWINPUTDEVICE) );

    return 0;
  }

  
//
// Try to grab the Platform object associated wtih this handle.  If we dont have one then
// its not a Nebulae window.
//
  Win32Platform* platform = ( Win32Platform* ) GetWindowLongPtr( hWnd, GWLP_USERDATA );
  if( !platform )
  {
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

//
// Grab the window object for this handle from the platform object.
// 
  Window* window = platform->FindWindowFromHandle( hWnd ).get();
  NE_ASSERT( window != NULL, "Unable to find the Win32Window associated with this handle" )(); 
  if( !window )
  {
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
  }

  switch( uMsg )
  {
  case WM_SIZE:
    {
      RECT rc;
      GetClientRect(hWnd, &rc);

      int32 width  = rc.right - rc.left;
      int32 height = rc.bottom - rc.top;
      int32 left   = rc.left;
      int32 top    = rc.top;

      window->MoveAndResize( left, top, width, height );
    }
    break;

  case WM_GETMINMAXINFO:
    // Prevent the window from going smaller than some arbitrary minimum size
    ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
    ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
    break;

  case WM_CLOSE:
    // Destroy the window
    window->Destroy();
    // Send the Quit application message
    PostQuitMessage( 0 );
    break;

  case WM_INPUT:
    {
      // Grab the current modKey flags.
      boost::uint32_t key_code_point = 0;
      Flags<ModKey>   modKeys        = GetModKeys( &platform->GetKeyboard() );
      

      // Determine how big the buffer should be.
      UINT bufferSize;
      GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, 
        sizeof(RAWINPUTHEADER) );
      
      // Obtain the data buffer.
      BYTE *buffer = new BYTE[bufferSize];// Create a buffer of the correct size - but see note below
      GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, 
        &bufferSize, sizeof(RAWINPUTHEADER) );
      
      RAWINPUT *raw = (RAWINPUT*)buffer; //< Cast buffer to a usable type.
            
      if( raw->header.dwType == RIM_TYPEMOUSE ) // Check for mouse movement.       
      {
        POINT mouse;
        mouse.x = -1, mouse.y = -1;

        if( raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE ) 
        { 
          GetCursorPos( &mouse );
          ScreenToClient( hWnd, &mouse );
        }	

        //@todo Handle MOUSE_MOVE_ABSOLUTE?? 

        Point cursor(mouse.x, mouse.y);

        if( window->IsPointInWindow( cursor ) )
        {
          // Check for mouse button presses.
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN ) window->ButtonPressed( mouse.x, mouse.y, Button1, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN ) window->ButtonPressed( mouse.x, mouse.y, Button2, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN ) window->ButtonPressed( mouse.x, mouse.y, Button3, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN ) window->ButtonPressed( mouse.x, mouse.y, Button4, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN ) window->ButtonPressed( mouse.x, mouse.y, Button5, modKeys );

          // Check for mouse button releases.
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP )   window->ButtonReleased( mouse.x, mouse.y, Button1, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP )   window->ButtonReleased( mouse.x, mouse.y, Button2, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP )   window->ButtonReleased( mouse.x, mouse.y, Button3, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP )   window->ButtonReleased( mouse.x, mouse.y, Button4, modKeys );
          if( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP )   window->ButtonReleased( mouse.x, mouse.y, Button5, modKeys );

          //@todo check if the mouse button was dragged? pWindow->ButtonDragged( cursor.x, cursor.y, Button1, modKeys );
          //@todo add support for MouseWheel; RI_MOUSE_WHEEL
        }
      } 
      else if( raw->header.dwType == RIM_TYPEKEYBOARD ) 
      {
        UINT virtualKey = raw->data.keyboard.VKey;
        UINT scanCode   = raw->data.keyboard.MakeCode;
        UINT flags      = raw->data.keyboard.Flags;
 
        if( virtualKey == 255 )
        {
          // discard "fake keys" which are part of an escaped sequence
          return 0;
        }
        else if( virtualKey == VK_SHIFT )
        {
          // correct left-hand / right-hand SHIFT
          virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
        }
        else if( virtualKey == VK_NUMLOCK )
        {
          // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
          scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
        }

        // e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
        // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
        const bool isE0 = ((flags & RI_KEY_E0) != 0);
        const bool isE1 = ((flags & RI_KEY_E1) != 0);
 
        if( isE1 )
        {
          // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
          // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
          scanCode = virtualKey == VK_PAUSE ? 0x45 : MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
        }

        switch( virtualKey )
        {
          // right-hand CONTROL and ALT have their e0 bit set
          case VK_CONTROL:
            if (isE0)
              virtualKey = VK_RCONTROL;
            else
              virtualKey = VK_LCONTROL;
            break;

          case VK_MENU:
            if (isE0)
              virtualKey = VK_RMENU;
            else
              virtualKey = VK_LMENU;
            break;

          // NUMPAD ENTER has its e0 bit set
          //case VK_RETURN:
          //  if (isE0)
          //    virtualKey = Keys::NUMPAD_ENTER;
          //  break;

          // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
          // corresponding keys on the NUMPAD will not.
          case VK_INSERT:
            if (!isE0)
              virtualKey = VK_NUMPAD0;
            break;

          //case VK_DELETE:
          //  if (!isE0)
          //    virtualKey = Keys::NUMPAD_DECIMAL;
          //  break;

          case VK_HOME:
            if (!isE0)
              virtualKey = VK_NUMPAD7;
            break;

          case VK_END:
            if (!isE0)
              virtualKey = VK_NUMPAD1;
            break;

          case VK_PRIOR:
            if (!isE0)
              virtualKey = VK_NUMPAD9;
            break;

          case VK_NEXT:
            if (!isE0)
              virtualKey = VK_NUMPAD3;
            break;

          // the standard arrow keys will always have their e0 bit set, but the
          // corresponding keys on the NUMPAD will not.
          case VK_LEFT:
            if (!isE0)
              virtualKey = VK_NUMPAD4;
            break;

          case VK_RIGHT:
            if (!isE0)
              virtualKey = VK_NUMPAD6;
            break;

          case VK_UP:
            if (!isE0)
              virtualKey = VK_NUMPAD8;
            break;

          case VK_DOWN:
            if (!isE0)
              virtualKey = VK_NUMPAD2;
            break;

          // NUMPAD 5 doesn't have its e0 bit set
          case VK_CLEAR:
            if (!isE0)
              virtualKey = VK_NUMPAD5;
            break;
        }

        // a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
        // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
        const bool wasUp        = ((flags & RI_KEY_BREAK) != 0);

        KeyCode engineKey      = VirtualKeyFromWindowsKey( virtualKey, modKeys );
        int32   convertedChars = platform->GetKeyboard().ToUnicode( engineKey, &key_code_point );

        // Send the message to the associated window.
        if( wasUp )
        {
          window->KeyReleased( engineKey, key_code_point, modKeys );
        } 
        else 
        {
          window->KeyPressed( engineKey, key_code_point, modKeys );
        }
        
        // Adjust the keyboard structure to reflect keypress.
        platform->GetKeyboard().SetKeyState( engineKey, !wasUp );
      }
    }
    break;

  default:
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
    
  }

  return 1;
}


Win32Platform::Win32Platform()
: Platform(), 
  m_pRegisterationUtility( NULL )
{}

Win32Platform::~Win32Platform()
{}

bool 
Win32Platform::Initiate()
{
  // Create the registeration utility & register the Nebulae window class.
	m_pRegisterationUtility = new ClassRegisterationUtility();
	m_pRegisterationUtility->Register( L"NebulaeWindowClass", NebulaeWndProc, ::GetModuleHandle( NULL ), NULL );

  // @note [jared.watt] This is here rather than in Platform::Intiate so that we dont have to call
  // Platform::Initiate() as thats an ugly way of coding and I cant think of a better solution 
  // right now :D
  m_fileSystem = std::make_shared<FileSystem >();

  m_keyboard.Fetch();

  // Create a Console window for us to send debug messages to.
  int hConHandle;
  HANDLE lStdHandle;
  FILE *fp;

  // allocate a console for this app
  AllocConsole();

  // redirect unbuffered STDOUT to the console
  lStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
  hConHandle = _open_osfhandle( (intptr_t) lStdHandle, _O_TEXT );
  fp = _fdopen( hConHandle, "w" );
  *stdout = *fp;

  setvbuf( stdout, NULL, _IONBF, 0 );
  
	return true;
}

void 
Win32Platform::Destroy()
{
  for( std::vector<WindowPtr >::iterator itr = m_windows.begin(); itr != m_windows.end(); ++itr )
  {
    ( *itr )->Destroy();
  }
  m_windows.clear();

  if( m_pRegisterationUtility ) delete m_pRegisterationUtility;

  m_fileSystem.reset();

  m_pRegisterationUtility = NULL;
  m_fileSystem            = NULL;
}

bool
Win32Platform::MessagePump()
{
  // This is the basic platform message pump.  If it receives a Quit message from the system it will return a non zero value.
  return (Win32Utils::MessagePump() != 0);
}

void 
Win32Platform::SetWorkingDirectory( const char* directory )
{
  // Setup the data paths.
  _chdir( directory );
}

Platform::WindowPtr
Win32Platform::CreateApplicationWindow(int x, int y, int w, int h, Platform::WindowPtr pParent)
{
	// Create new Window.
	Win32Window* pWindow = new Win32Window( L"NebulaeWindowClass", NULL );
	if( pWindow ) {
	  pWindow->MoveAndResize( x, y, w, h );
	  pWindow->SetCaption( L"Nebulae" );
	  pWindow->Initiate( this );
	
    m_windows.push_back( WindowPtr(pWindow) );

    return m_windows.back();
  }
	
  return NULL;
}

void 
Win32Platform::DestroyWindow( WindowPtr window )
{
  std::vector<WindowPtr >::iterator it = std::find( m_windows.begin(), m_windows.end(), window );
  if( it != m_windows.end() )
  {
    window->Destroy();
    m_windows.erase( it );
  }
}

Platform::WindowPtr
Win32Platform::FindWindowFromHandle( HWND handle )
{
  for( std::vector<WindowPtr >::iterator itr = m_windows.begin(); itr != m_windows.end(); ++itr )
  {
    std::shared_ptr<Win32Window > window = std::static_pointer_cast<Win32Window >( *itr );

    if( window->GetWnd() == handle )
    {
      return ( *itr );
    }
  }

  return WindowPtr( NULL );
}