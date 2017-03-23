#ifndef GUIINPUTLISTENER_H__ 
#define GUIINPUTLISTENER_H__ 

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>
#include <Nebulae/Beta/Gui/GuiManager.h>


namespace Nebulae {

class GuiInputListener : public Nebulae::InputListener
{
private:
  GuiManager& m_gui;

  public:
    GuiInputListener( GuiManager& gui ) : m_gui(gui) {}
    virtual ~GuiInputListener() {}

    virtual void KeyPressed( Nebulae::KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys ) override {
      m_gui.HandleEvents( Nebulae::GuiManager::KEYPRESS, keyCode, key_code_point, modKeys, Point(0,0), Point(0,0) );
    }

    virtual void KeyReleased( Nebulae::KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys ) override {
      m_gui.HandleEvents( Nebulae::GuiManager::KEYRELEASE, keyCode, key_code_point, modKeys, Point(0,0), Point(0,0) );
    }

    virtual void MousePressed( Nebulae::MouseButton button, int x, int y, Flags<ModKey> modKeys ) override {
      m_gui.HandleEvents( Nebulae::GuiManager::TOUCHDOWN, Nebulae::VKC_UNKNOWN, 0, modKeys, Point(x,y), Point(0,0) );
    }

    virtual void MouseReleased( Nebulae::MouseButton button, int x, int y, Flags<ModKey> modKeys ) override {
      m_gui.HandleEvents( Nebulae::GuiManager::TOUCHUP, Nebulae::VKC_UNKNOWN, 0, modKeys, Point(x,y), Point(0,0) );
    }

    virtual void MouseMove( Nebulae::MouseButton button, int x, int y, Flags<ModKey> modKeys ) override {
      // @todo [jared.watt 11.03.2014] Need to handle the relative mouse coordinate movement somehow?
      m_gui.HandleEvents( Nebulae::GuiManager::TOUCHDRAG, Nebulae::VKC_UNKNOWN, 0, modKeys, Point(x,y), Point() );
    }

};

}

#endif GUIINPUTLISTENER_H__ 