#ifndef __CONTROLLER_BASESCREEN_H__
#define __CONTROLLER_BASESCREEN_H__

#include <Config.h>
#include "ScreenController.h"
#include "Controller/Dispatcher.h"
#include "Controller/PropertyArray.h"

#include <Nebulae/Nebulae.h>

namespace Nebulae {
  class Widget;
  class GuiManager;
}

namespace Game {
  
class AudioController;
class LuaInterpreter;
class Model;
class PropertyArray;
class View;

class BaseScreen : public Screen
///
/// Each screen represents a particular application state and should encapsulate anything
/// related to the execution of that state.  When a phase of the application should 
/// be considered an application state.
///
/// "Squidish" attempts to use a MVC style system thus each screen should contain
/// the collection of model, view & controller objects and is primarily responsible
/// with the setup of whichever views and controllers that are required for that 
/// game phase. 
///
{
  public:
    typedef std::shared_ptr<Nebulae::FileSystem >        FileArchivePtr;

    typedef std::shared_ptr<Model >                      ModelPointer;
    typedef std::shared_ptr<Nebulae::GuiManager >        GuiControllerPointer; //< This is actually a "GuiModel" style object

    typedef std::shared_ptr<AudioController >            AudioControllerPointer;
    typedef std::shared_ptr<LuaInterpreter >             ScriptControllerPointer;

    typedef std::shared_ptr<Nebulae::RenderSystem >      RenderSystemPointer;
    typedef std::shared_ptr<View >                       ViewPointer;

  public:
    struct BaseScreenEventResolver;
  
  protected:
    FileArchivePtr                 m_fileSystem;      ///< The FileSystem control object.
    Dispatcher&                    m_dispatcher;      ///< Dispatcher for ui interaction.
    AudioControllerPointer         m_audioController; ///< The audio control object.
    GuiControllerPointer           m_guiController;   ///< Shared reference to the GUI Manager object.
    ModelPointer                   m_model;           ///< Shared reference to Model object.
    ViewPointer                    m_view;            ///< Shared reference to View object.
    PropertyArray                  m_properties;      ///< The screen specific property list.
    std::vector<Nebulae::Widget* > m_widgets;         ///< The ui widgets.

    public:
      /** \name Structors */ ///@{
      BaseScreen( const std::string& name, ScreenController* owner, const FileArchivePtr& fileSystem, 
        Dispatcher& dispatcher, const AudioControllerPointer& audioPlayer, const GuiControllerPointer& guiMgr, 
        const ModelPointer& model, const ViewPointer& view );

      virtual ~BaseScreen();
      //@}
    
      /** \name Mutators */ ///@{
      virtual void Clear();
      virtual void Init();
      virtual bool Load( const char* filename );
      virtual bool LoadScene( const char* filename = NULL );
      virtual Nebulae::Widget* LoadUi( const char* filename = NULL );
      virtual void Enter( const std::vector<std::string>& args );
      virtual void Exit();
      virtual void Update( float elapsed );
      virtual void AddEvent( Event event );
      //@}

      /** \name Accessors */ ///@{
      ModelPointer           GetModel();           
      ViewPointer            GetView();            
      AudioControllerPointer GetAudioController(); 
      GuiControllerPointer   GetGuiController();
      PropertyArray&         GetProperties();      
      //@}

    protected:

      /** \name Mutators */ ///@{
      virtual void OnInit();
      virtual void OnEnter();
      virtual void OnExit();
      virtual bool OnKeyPressed(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
      virtual bool OnKeyReleased(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
      virtual bool OnTouchDown(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
      virtual bool OnTouchUp(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
      virtual bool OnTouchDrag(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
      virtual void OnUpdate( float elapsed );

      virtual void SetupScreenFromDefaultParameters();
      virtual void ProcessDispatcher( Dispatcher& dispatcher );
      virtual bool InvokeAction( const std::string& context );
      //@}

};

}

#endif // __CONTROLLER_BASESCREEN_H__