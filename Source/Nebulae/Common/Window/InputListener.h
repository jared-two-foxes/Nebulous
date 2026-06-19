#ifndef NEBULAE_COMMON_WINDOW_INPUTLISTENER_H_
#define NEBULAE_COMMON_WINDOW_INPUTLISTENER_H_

namespace Nebulae
{

enum MouseButton
{
  Button1 = 0,
  Button2,
  Button3,
  Button4,
  Button5,
  Count
};

// Adpated from SDLKey enum in SDL_keysym.h of the SDL library.
NE_FLAG_TYPE( ModKey );
extern const ModKey MOD_KEY_NONE;         ///< No modifier key.
extern const ModKey MOD_KEY_LSHIFT;       ///< The left Shift key.
extern const ModKey MOD_KEY_RSHIFT;       ///< The right Shift key.
extern const ModKey MOD_KEY_LCTRL;        ///< The left Control key.
extern const ModKey MOD_KEY_RCTRL;        ///< The right Control key.
extern const ModKey MOD_KEY_LALT;         ///< The left Alt key.
extern const ModKey MOD_KEY_RALT;         ///< The right Alt key.
extern const ModKey MOD_KEY_LMETA;        ///< The left Meta key.
extern const ModKey MOD_KEY_RMETA;        ///< The right Meta key.
extern const ModKey MOD_KEY_NUM;          ///< The Num Lock key.
extern const ModKey MOD_KEY_CAPS;         ///< The Caps Lock key.
extern const ModKey MOD_KEY_MODE;         ///< The Mode key.
extern const Flags<ModKey> MOD_KEY_CTRL;  ///< Either Control key.
extern const Flags<ModKey> MOD_KEY_SHIFT; ///< Either Shift key.
extern const Flags<ModKey> MOD_KEY_ALT;   ///< Either Alt key.
extern const Flags<ModKey> MOD_KEY_META;  ///< Either Meta key.

// left: raw->data.keyboard.Flags & RI_KEY_E0, right: raw->data.keyboard.Flags & RI_KEY_E1
// struct Key
//{
//	unsigned short m_nKeyCode;
//	unsigned short m_nFlags;
// };

class InputListener
{
public:
  virtual ~InputListener() {}

  virtual void KeyPressed( [[maybe_unused]] KeyCode code, [[maybe_unused]] uint32 repeat,
                           [[maybe_unused]] Flags<ModKey> mod_keys )
  {
  }
  virtual void KeyReleased( [[maybe_unused]] KeyCode code, [[maybe_unused]] uint32 repeat,
                            [[maybe_unused]] Flags<ModKey> mod_keys )
  {
  }

  virtual void MousePressed( [[maybe_unused]] MouseButton button, [[maybe_unused]] int32 x, [[maybe_unused]] int32 y,
                             [[maybe_unused]] Flags<ModKey> mod_keys )
  {
  }
  virtual void MouseReleased( [[maybe_unused]] MouseButton button, [[maybe_unused]] int32 x, [[maybe_unused]] int32 y,
                              [[maybe_unused]] Flags<ModKey> mod_keys )
  {
  }
  virtual void MouseMove( [[maybe_unused]] MouseButton button, [[maybe_unused]] int32 x, [[maybe_unused]] int32 y,
                          [[maybe_unused]] Flags<ModKey> mod_keys )
  {
  }

}; // InputListener

} // namespace Nebulae

#endif // NEBULAE_COMMON_WINDOW_INPUTLISTENER_H_