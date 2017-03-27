#ifndef NE_GUI_INPUTLISTENER_H__ 
#define NE_GUI_INPUTLISTENER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>
#include <Nebulae/Beta/Gui/GuiManager.h>

namespace Nebulae {

class UserInterfaceInputListener : public Nebulae::InputListener
{
public:
  typedef std::shared_ptr<GuiManager > UserInterfaceControllerPtr;
private:
  UserInterfaceControllerPtr m_gui;

  public:
    UserInterfaceInputListener( const UserInterfaceControllerPtr& gui ) : m_gui(gui) {}
    
    virtual void KeyPressed( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey > modKeys ) override
    {
      m_gui->HandleEvents( GuiManager::KEYPRESS, keyCode, key_code_point, modKeys, Point(0,0), Point(0,0) );
    }

    virtual void KeyReleased( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_gui->HandleEvents( GuiManager::KEYRELEASE, keyCode, key_code_point, modKeys, Point(0,0), Point(0,0) );
    }

    virtual void MousePressed( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_gui->HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, modKeys, Point(x,y), Point(0,0) );
    }

    virtual void MouseReleased( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
    {
      m_gui->HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN, 0, modKeys, Point(x,y), Point(0,0) );
    }

    virtual void MouseMove( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys )  override
    {
      m_gui->HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, 0, modKeys, Point(x,y), Point() );
    }

};

}

#endif // NE_GUI_INPUTLISTENER_H__ 