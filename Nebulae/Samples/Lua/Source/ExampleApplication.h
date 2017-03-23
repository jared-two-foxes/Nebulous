#ifndef __EXAMPLEAPPLICATION_H__
#define __EXAMPLEAPPLICATION_H__

#include <Nebulae/Nebulae.h>
#include <Nebulae/Beta/Application/Application.h>


namespace Nebulae {
  
class AudioManager;
class DebugConsole;
class GuiManager;
class LuaInterpreter;
class WidgetRenderer;

class ExampleApplication : public Application
{
public:
  typedef boost::shared_ptr<AudioManager >   AudioControllerPtr;
  typedef boost::shared_ptr<GuiManager >     GuiControllerPtr;
  typedef boost::shared_ptr<LuaInterpreter > ScriptInterpreterPtr;
   
private:
  AudioControllerPtr   m_audioController;   ///< The Audio controller.
  GuiControllerPtr     m_guiController;     ///< The Gui controller.
  WidgetRenderer*      m_widgetRenderer; 
  ScriptInterpreterPtr m_scriptInterpreter; ///< The script interpreter.
  DebugConsole*        m_debugConsole;      ///< The debug console element.
  

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

};

}

#endif // __EXAMPLEAPPLICATION_H__