
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Platform/Keyboard.h>

#include <bitset>

using namespace Nebulae;


uint16 
WindowsKeyFromVirtualKey( KeyCode keyCode )
{
  uint16 retval = 0; 

  switch( keyCode )
  {
    case VKC_ESCAPE:          retval = VK_ESCAPE; break;
    case VKC_MINUS:           retval = VK_SUBTRACT; break;
    case VKC_EQUALS:          retval = VK_OEM_PLUS; break;
    case VKC_BACKSPACE:       retval = VK_BACK; break;
    case VKC_TAB:             retval = VK_TAB; break;
    case VKC_LEFTBRACKET:     retval = VK_OEM_4; break;
    case VKC_RIGHTBRACKET:    retval = VK_OEM_6; break;
    case VKC_RETURN:          retval = VK_RETURN; break;
    case VKC_LCTRL:           retval = VK_LCONTROL; break;
    case VKC_SEMICOLON:       retval = VK_OEM_1; break;
    case VKC_QUOTE:           retval = VK_OEM_7; break;
    case VKC_LSHIFT:          retval = VK_LSHIFT; break;
    case VKC_BACKSLASH:       retval = VK_OEM_2; break;
    case VKC_COMMA:           retval = VK_OEM_COMMA; break;
    case VKC_PERIOD:          retval = VK_OEM_PERIOD; break;
    case VKC_RSHIFT:          retval = VK_RSHIFT; break;
    case VKC_KP_MULTIPLY:     retval = VK_MULTIPLY; break;
    case VKC_LALT:            retval = VK_LMENU; break;
    case VKC_SPACE:           retval = VK_SPACE; break;
    case VKC_CAPSLOCK:        retval = VK_CAPITAL; break;
    case VKC_F1:              retval = VK_F1; break;
    case VKC_F2:              retval = VK_F2; break;
    case VKC_F3:              retval = VK_F3; break;
    case VKC_F4:              retval = VK_F4; break;
    case VKC_F5:              retval = VK_F5; break;
    case VKC_F6:              retval = VK_F6; break;
    case VKC_F7:              retval = VK_F7; break;
    case VKC_F8:              retval = VK_F8; break;
    case VKC_F9:              retval = VK_F9; break;
    case VKC_F10:             retval = VK_F10; break;
    case VKC_NUMLOCK:         retval = VK_NUMLOCK; break;
    case VKC_SCROLLOCK:       retval = VK_SCROLL; break;
    case VKC_KP7:             retval = VK_NUMPAD7; break;
    case VKC_KP8:             retval = VK_NUMPAD8; break;
    case VKC_KP9:             retval = VK_NUMPAD9; break;
    case VKC_KP4:             retval = VK_NUMPAD4; break;
    case VKC_KP5:             retval = VK_NUMPAD5; break;
    case VKC_KP6:             retval = VK_NUMPAD6; break;
    case VKC_KP_PLUS:         retval = VK_ADD; break;
    case VKC_KP1:             retval = VK_NUMPAD1; break;
    case VKC_KP2:             retval = VK_NUMPAD2; break;
    case VKC_KP3:             retval = VK_NUMPAD3; break;
    case VKC_KP0:             retval = VK_NUMPAD0; break;
    case VKC_F11:             retval = VK_F11; break;
    case VKC_F12:             retval = VK_F12; break;
    case VKC_F13:             retval = VK_F13; break;
    case VKC_F14:             retval = VK_F14; break;
    case VKC_F15:             retval = VK_F15; break;
    case VKC_RCTRL:           retval = VK_RCONTROL; break;
    case VKC_KP_DIVIDE:       retval = VK_DIVIDE; break;
    case VKC_RALT:            retval = VK_RMENU; break;
    case VKC_PAUSE:           retval = VK_PAUSE; break;
    case VKC_HOME:            retval = VK_HOME; break;
    case VKC_UP:              retval = VK_UP; break;
    case VKC_LEFT:            retval = VK_LEFT; break;
    case VKC_RIGHT:           retval = VK_RIGHT; break;
    case VKC_END:             retval = VK_END; break;
    case VKC_DOWN:            retval = VK_DOWN; break;
    case VKC_INSERT:          retval = VK_INSERT; break;
    case VKC_DELETE:          retval = VK_DELETE; break;

    default: 
      retval = uint16( keyCode );
  }

  return retval;
}

void 
ConvertToWindowsArray( uint8* keyboardState, BYTE* out )
{
  memset( out, 0, 256*sizeof(BYTE) );

  for( uint32 vkcode = VKC_BACKSPACE; vkcode < VKC_LAST; vkcode++ )
  {
    uint16 index = WindowsKeyFromVirtualKey( KeyCode(vkcode) );
    if( index != 0 && index <= 0xFF )
    {
      out[index] = keyboardState[vkcode];
    }
  }
}

void 
SetKeyState( uint8* keyboardState, KeyCode virtualKey, bool down, bool toggled )
{
  if( toggled )
  {
    keyboardState[virtualKey] |= 1 << 0;
  }
  else
  {
    keyboardState[virtualKey] &= ~( 1 << 0 );
  }

  if( down )
  {
    keyboardState[virtualKey] |= 1 << 7;
  }
  else
  {
    keyboardState[virtualKey] &= ~( 1 << 7 );
  }
}

Keyboard::Keyboard() 
{}

Keyboard::~Keyboard() 
{}

bool 
Keyboard::IsKeyDown( KeyCode virtualKey ) const
{ 
  return std::bitset<sizeof(uint8)*CHAR_BIT>(m_keyboardState[virtualKey]).test( 7 );
}

bool
Keyboard::IsKeyUp( KeyCode virtualKey ) const
{ 
  return !std::bitset<sizeof(uint8)*CHAR_BIT>(m_keyboardState[virtualKey]).test( 7 );
}

bool
Keyboard::IsToggled( KeyCode virtualKey ) const
{ 
  return std::bitset<sizeof(uint8)*CHAR_BIT>(m_keyboardState[virtualKey]).test( 0 );
}

void 
Keyboard::SetKeyState( KeyCode virtualKey, bool down )
{ 
  bool toggled = IsToggled(virtualKey);
  if( down ) toggled = !toggled;
  ::SetKeyState( m_keyboardState, virtualKey, down, toggled );

  if( virtualKey == VKC_LSHIFT || virtualKey == VKC_LSHIFT )
  {
    ::SetKeyState( m_keyboardState, VKC_SHIFT, down, toggled );
  }
  else if( virtualKey == VKC_LCTRL || virtualKey == VKC_RCTRL )
  {
    ::SetKeyState( m_keyboardState, VKC_CTRL, down, toggled );
  }
  else if( virtualKey == VKC_LALT || virtualKey == VKC_RALT )
  {
    ::SetKeyState( m_keyboardState, VKC_ALT, down, toggled );
  }
}

void 
Keyboard::GetAllStates( uint8* state )
{
  memcpy( state, m_keyboardState, 256 );
}

void 
Keyboard::Fetch()
{
  memset( m_keyboardState, 0, sizeof(m_keyboardState) );

  // Update the internal key states based upon current physical status.
  for( uint32 vkcode = VKC_BACKSPACE; vkcode < VKC_LAST; vkcode++ )
  {
    uint16 index = WindowsKeyFromVirtualKey( KeyCode(vkcode) );
    if( index != 0 && index <= 0xFF )
    {
      SHORT keyState = GetKeyState( index );
      std::bitset<sizeof(SHORT)*CHAR_BIT> bitset( keyState );
      bool toggled = bitset.test( 0 );
      bool down    = bitset.test( 15 );

      ::SetKeyState( m_keyboardState, KeyCode(vkcode), down, toggled );
    }
  }
}

int32 
Keyboard::ConvertKeycodeToAscii( KeyCode keycode, Flags<ModKey> mod_keys, uint8* asciiChar )
{
  // Check if key is alnum.
  if( keycode > VKC_DELETE )
  {
    return 0;
  }

  bool shift = mod_keys & MOD_KEY_LSHIFT;
  bool caps  = mod_keys & MOD_KEY_CAPS;
  
  if( !shift && keycode >= 'A' && keycode <= 'Z' )
  {
    *asciiChar = std::tolower( keycode );
  }
  else
  {
    *asciiChar = keycode;
  }
  
  return 1;
}

int32 
Keyboard::ToUnicode( KeyCode keycode, uint32* unicodeChar )
{
  BYTE kb[256];
  ConvertToWindowsArray( m_keyboardState, kb );

  BYTE kb2[256];
  GetKeyboardState( kb2 );

  // Update the internal key states based upon current physical status.
  for( uint32 win_key = VK_BACK; win_key < 256; win_key++ )
  {
    BYTE a = kb[win_key];
    BYTE b = kb2[win_key];

    if( a != b )
    {
      int breakpoint = 12;
    }
  }

  WCHAR  uc[5]          = {};
  uint32 key_code_point = 0;
  UINT   virtualKey     = WindowsKeyFromVirtualKey( keycode );
  UINT   scan_code      = MapVirtualKey( virtualKey, MAPVK_VK_TO_VSC );

  int32 count = ::ToUnicode( virtualKey, scan_code, kb, uc, 4, 0 );

  // copy characters into 'unicodeChar'
  if( count > 0 )
  {
    memcpy( unicodeChar, uc, sizeof(uint32) );
  }

  return count;
}

bool 
Keyboard::IsPrintableKey( KeyCode& key, Flags<ModKey> mod_keys )
{
  if( !(mod_keys & MOD_KEY_NUM) )
  {
    Keyboard::KeypadKeyToPrintable( key, mod_keys );
  }

  if( key >= VKC_DELETE )
  {
    return false;
  }

  return (std::isprint( char(key) ) > 0);
}

void 
Keyboard::KeypadKeyToPrintable( KeyCode& key, Flags<ModKey> mod_keys )
{
  using namespace Nebulae;

  if( VKC_KP0 <= key && key <= VKC_KP9 && (mod_keys & MOD_KEY_NUM) ) 
  {
    key = KeyCode(VKC_0 + (key - VKC_KP0));
  } 
  else 
  {
    switch( key ) 
    {
      case VKC_KP_PERIOD:
        if (mod_keys & MOD_KEY_NUM) key = VKC_PERIOD;
        break;
      case VKC_KP_DIVIDE:   key = VKC_SLASH;    break;
      case VKC_KP_MULTIPLY: key = VKC_ASTERISK; break;
      case VKC_KP_MINUS:    key = VKC_MINUS;    break;
      case VKC_KP_PLUS:     key = VKC_PLUS;     break;
      case VKC_KP_EQUALS:   key = VKC_EQUALS;   break;
      default: break;
    }
  }
}