#ifndef NEBULAE_BETA_GUI_USERINTERFACEINPUTLISTENER_H_
#define NEBULAE_BETA_GUI_USERINTERFACEINPUTLISTENER_H_

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>

namespace Nebulae
{

class UserInterfaceInputListener : public Nebulae::InputListener
{
public:
  typedef std::shared_ptr<GuiManager> UserInterfaceControllerPtr;

private:
  UserInterfaceControllerPtr m_gui;

public:
  UserInterfaceInputListener( const UserInterfaceControllerPtr& gui ) : m_gui( gui ) {}

  void KeyPressed( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
  {
    m_gui->HandleEvents( GuiManager::KEYPRESS, keyCode, key_code_point, modKeys, Point( 0, 0 ), Point( 0, 0 ) );
  }

  void KeyReleased( Nebulae::KeyCode keyCode, uint32 key_code_point, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
  {
    m_gui->HandleEvents( GuiManager::KEYRELEASE, keyCode, key_code_point, modKeys, Point( 0, 0 ), Point( 0, 0 ) );
  }

  void MousePressed( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
  {
    m_gui->HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN, 0, modKeys, Point( x, y ), Point( 0, 0 ) );
  }

  void MouseReleased( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
  {
    m_gui->HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN, 0, modKeys, Point( x, y ), Point( 0, 0 ) );
  }

  void MouseMove( Nebulae::MouseButton button, int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys ) override
  {
    m_gui->HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN, 0, modKeys, Point( x, y ), Point() );
  }
};

} // namespace Nebulae

#endif // NEBULAE_BETA_GUI_USERINTERFACEINPUTLISTENER_H_