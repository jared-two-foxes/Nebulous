#ifndef __EXAMPLEAPPLICATION_H__
#define __EXAMPLEAPPLICATION_H__

#include <Config.h>
#include <Controller/Dispatcher.h>

#include <Nebulae/Nebulae.h>
#include <Nebulae/Beta/Application/Application.h>

namespace Nebulae {
  class GuiManager;
  class Widget;
  class TextControl;
}

namespace Game {

class AudioController;
class Model;
class ScreenController;
class View;

class ExampleApplication : public Nebulae::Application
{
public:
  typedef std::shared_ptr<Nebulae::GuiManager >   GuiControllerPtr;

private:
  std::shared_ptr<AudioController >  m_audioController; ///< The Audio controller.
  GuiControllerPtr                   m_guiController;   ///< The Gui controller.
  std::shared_ptr<Model >            m_model;           ///< The model object.
  std::shared_ptr<View >             m_view;            ///< The view object.
  std::shared_ptr<ScreenController > m_controller;      ///< The controller object.
  Dispatcher                         m_dispatcher;      ///< The controller for routing events.

#ifndef NE_RETAIL
  bool                               m_show_fps;
  Nebulae::TextControl*              m_fps_label;
  bool                               m_show_profile_dump;
  Nebulae::TextControl*              m_profile_label;
#endif

  public:
    /** \name Structors */ ///@{
    ExampleApplication();
    ~ExampleApplication();
    //@}

    /** \name Mutators */ ///@{
    void Init();
    virtual void ProcessSimulation( uint64 elapsed );
    virtual void ProcessFrame( uint64 elapsed );
   //@}

#ifndef NE_RETAIL
    void ToggleFPS();
    void ToggleProfiler();
#endif

  private:
    Nebulae::Widget* LoadWidget( const char* filename );
    
};

}

#endif // __EXAMPLEAPPLICATION_H__