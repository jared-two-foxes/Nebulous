#ifndef WINDOW_WIN32_KEYBOARD_H__
#define WINDOW_WIN32_KEYBOARD_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Keyboard
///
/// This class is responsible for maintaining the current state of all keyboard keys.  
/// It currently uses a char array where each key maps to a single char and the value of 
/// char is dependent upon the key state.  The following convention has been stolen from Win32
/// and is used:
///   If the high order bit is 1, the key is down; otherwise, it is up.
///   If the low-order bit is 1, the key is toggled; otherwise it is off.  This is useful
///   for keys such as the CAPS LOCK key.
/// 
/// Currently only a win32 implementation is provided. It uses the windows Virtual Key
/// system for key identification which are provided by CasualCore.  Currently due to the way that
/// CasualCore processes windows events (it is restricted to passing a single windows event per frame) 
/// it will pass one KeyEvent to the game states at a time and the remaining KeyEvents will remain in 
/// the Windows Message queue until they are processed thus things like correct SHIFT button states 
/// are lost.
///
{
private:
  uint8 m_keyboardState[VKC_LAST]; ///< key status

  public:
    Keyboard();
    ~Keyboard();

    /** Updates the internal values of the current keyboard state.
     */
    void Fetch();

    /** Returns true if the key corresponding to keycode is down.
     */
    bool IsKeyDown( KeyCode keycode ) const;

    /** Returns true if the key corresponding to keycode is up.
     */
    bool IsKeyUp( KeyCode keycode ) const;

    /** Returns true if the key corresponding to keycode is currently toggled on.
     */
    bool IsToggled( KeyCode keycode ) const;

    void SetKeyState( KeyCode keycode, bool down );
    
    void GetAllStates( uint8* state );


    /** Converts a given keycode into an ascii code.  Assumes that fetch
     *  has already been called.
     */
    static int32 ConvertKeycodeToAscii( KeyCode keycode, Flags<ModKey> mod_keys, uint8* asciiChar );

    
    int32 ToUnicode( KeyCode keycode, uint32* unicodeChar );

    static bool IsPrintableKey( KeyCode& key, Flags<ModKey> mod_keys );

    static void KeypadKeyToPrintable( KeyCode& key, Flags<ModKey> mod_keys );

};

}

#endif // WINDOW_WIN32_KEYBOARD_H__ 