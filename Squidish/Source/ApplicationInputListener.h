#ifndef __GAME_APPLICATIONINPUTLISTENER_H__
#define __GAME_APPLICATIONINPUTLISTENER_H__ 

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>

namespace Game
{

class ApplicationInputListener : public Nebulae::InputListener
///
///
///
{
private:
  ExampleApplication& m_application;

  public:
    ApplicationInputListener( ExampleApplication& application ) : m_application(application) {}
    
    virtual void KeyPressed( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {}

    virtual void KeyReleased( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      if( keyCode == Nebulae::VKC_SPACE )
      {
        m_application.ToggleProfiler();
      }

      //m_application.OnKeyReleased( keyCode, modKeys );
    }

    virtual void MousePressed( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {}

    virtual void MouseReleased( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {}

    virtual void MouseMove( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {}

};

}

#endif // __GAME_APPLICATIONINPUTLISTENER_H__ 