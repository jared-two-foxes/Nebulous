#ifndef GAME_INPUTLISTENER_H__ 
#define GAME_INPUTLISTENER_H__ 

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>
#include <Nebulae/Beta/Gui/GuiManager.h>

#include <ScreenController.h>

#include <Model/Model.h>

namespace Game
{

class GameInputListener : public Nebulae::InputListener
///
/// @todo [jared.watt 11.03.2014]
///   should we have the screen controller update the gui manager input?
///
{
private:
  ScreenController& m_screenController;
  Model&            m_model;

  public:
    GameInputListener( ScreenController& screenController, Model& model ) : m_screenController(screenController), m_model(model) {}
    virtual ~GameInputListener() {}

    virtual void KeyPressed( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_screenController.CurrentScreen().OnKeyPressed( keyCode, modKeys );
    }

    virtual void KeyReleased( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_screenController.CurrentScreen().OnKeyReleased( keyCode, modKeys );
    }

    virtual void MousePressed( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_screenController.CurrentScreen().OnTouchDown( x, y, modKeys );
    }

    virtual void MouseReleased( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_screenController.CurrentScreen().OnTouchUp( x, y, modKeys );
    }

    virtual void MouseMove( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_screenController.CurrentScreen().OnTouchDrag( x, y, modKeys );
    }

};

}

#endif // GAME_INPUTLISTENER_H__ 