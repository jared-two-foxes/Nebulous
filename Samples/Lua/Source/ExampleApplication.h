#ifndef __EXAMPLEAPPLICATION_H__
#define __EXAMPLEAPPLICATION_H__


#define USE_BATCHER


#include <Nebulae/Nebulae.h>
#include <Nebulae/Beta/Application/Application.h>


namespace Nebulae {

class AudioManager;
class DebugConsole;
class GuiManager;
class LuaInterpreter;
class SpriteBatch;
class WidgetRenderer;

class ExampleApplication : public Application
{
public:
  typedef std::shared_ptr<GuiManager >     GuiControllerPtr;
  typedef std::shared_ptr<WidgetRenderer > WidgetRendererPtr;
  typedef std::shared_ptr<LuaInterpreter > ScriptInterpreterPtr;
  typedef std::shared_ptr<DebugConsole >   DebugConsolePtr;

private:

#ifdef NEBULAE_AUDIO
  typedef std::shared_ptr<AudioManager > AudioManagerPtr;
  AudioManagerPtr      m_audioManager;
#endif

#ifdef USE_BATCHER
  typedef std::shared_ptr<SpriteBatch > SpriteBatcherPtr;
  SpriteBatcherPtr m_batcher;
#endif 

  GuiControllerPtr     m_guiController;     ///< The Gui controller.
  WidgetRendererPtr    m_widgetRenderer;
  ScriptInterpreterPtr m_scriptInterpreter; ///< The script interpreter.
  DebugConsolePtr      m_debugConsole;      ///< The debug console element.
  

  public:

    /** \name Mutators */ ///@{
    void Init();
    virtual void ProcessSimulation( uint64 elapsed );
    virtual void ProcessFrame( uint64 elapsed );
    //@}

};

}

#endif // __EXAMPLEAPPLICATION_H__