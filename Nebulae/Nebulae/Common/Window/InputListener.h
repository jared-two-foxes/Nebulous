#ifndef NEBULAE_COMMON_WINDOW_INPUTLISTENER_H__
#define NEBULAE_COMMON_WINDOW_INPUTLISTENER_H__

namespace Nebulae {

enum MouseButton {
	Button1 = 0,
	Button2,
	Button3,
	Button4,
	Button5,
	Count
};

// Adpated from SDLKey enum in SDL_keysym.h of the SDL library.
NE_FLAG_TYPE(ModKey);
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

//left: raw->data.keyboard.Flags & RI_KEY_E0, right: raw->data.keyboard.Flags & RI_KEY_E1
//struct Key 
//{
//	unsigned short m_nKeyCode;
//	unsigned short m_nFlags; 
//};

class InputListener
{
public:
  virtual ~InputListener() {}

  virtual void KeyPressed( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys ) {}
  virtual void KeyReleased( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys ) {}

  virtual void MousePressed( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}
  virtual void MouseReleased( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}
  virtual void MouseMove( MouseButton button, int32 x, int32 y, Flags<ModKey> modKeys ) {}

}; //InputListener

} //Nebulae

#endif // NEBULAE_COMMON_WINDOW_INPUTLISTENER_H__