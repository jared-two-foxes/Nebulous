
#include "BaseScreen.h"

#include <Model/Model.h>
#include <View/View.h>

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/WidgetSerializer.h>

using namespace Nebulae;
using namespace Game;


BaseScreen::BaseScreen( const std::string& name, ScreenController* owner, const FileArchivePtr& fileSystem,
  Dispatcher& dispatcher, const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr, 
  const ModelPointer& model, const ViewPointer& view ) 
///
/// Constructor.
///
  : Screen( name, owner ),
    m_fileSystem( fileSystem ),
    m_dispatcher( dispatcher ),
    m_audioController( audioController ),
    m_guiController( guiMgr ),
    m_model( model ),
    m_view( view ),
    m_properties(),
    m_widgets()
{}


BaseScreen::~BaseScreen()
///
/// Destructor
///
{}


void
BaseScreen::Clear()
///
/// Empties the model & view controllers of their contents and destroys
/// any created ui element.
///
/// @return
///   Nothing.
///
{
  std::for_each(m_widgets.begin(), m_widgets.end(), [&](Widget* widget){ 
    m_guiController->WidgetDying( widget ); 
  });

  m_widgets.clear();

  m_model->Clear();
  m_view->Clear();
}


void
BaseScreen::Init()
{
  std::string filename;
  filename.append( "level/" );
  filename.append( GetName() );
  filename.append( ".json" );

  if( !Load(filename.c_str()) )
  { 
    OnInit(); 
  }
}


bool 
BaseScreen::Load( const char* filename )
///
/// Load's the property values for this screen.
///
/// @param filename
///   the name of the file to load.
///
/// @return 
///   Nothing.
///
{
  File* is = m_fileSystem->Open( SQUID_DEFAULT_ROOTDEVICE, filename );
  if( is != NULL ) {
    bool loaded = m_properties.Load( *is );
    if( !loaded ) {
      return false;
    }
  }

  return true;
}


bool 
BaseScreen::LoadScene( const char* filename )
///
/// Will load a scene file of a given name into the model object, multiple scene files
/// can be loaded at a time.
///
/// @param filename
///   The name of the scene file to load.
///
/// @return
///   true  - the loading process was successful
///   false - the scene file failed to load.
///
{
  if( filename == NULL || strlen(filename) == 0 ) 
  {
    return false;
  }

  File* file = m_fileSystem->Open( SQUID_DEFAULT_ROOTDEVICE, filename );
  if( file != NULL )
  {
    m_model->LoadSceneFile( *file );
  }
  
  return true;
}


Widget* 
BaseScreen::LoadUi( const char* filename )
///
/// Loads a ui layout from file and sets the root element of the layout to
/// the member variable m_ui.  This assumes that there is a single root element
/// and that the only one layout can be loaded at a time.  If a layout has
/// already been loaded it will be unloaded before the next layout is loaded
/// in.  If the layout fails and there exists a previous layout, that ui layout
/// will be preserved.
///
/// @param filename
///   The file in which the layout can be found.
///   
/// @return
///   A pointer to the root Widget if loaded successful, else NULL.
///
{
  NE_ASSERT( filename, "filename is not specified" )( filename );

  if( filename == NULL || strlen(filename) == 0 ) 
  {
    return NULL;
  }

  Widget* widget = NULL;
  File*   stream = m_fileSystem->Open( SQUID_DEFAULT_ROOTDEVICE, filename );
  if( stream != NULL ) 
  {
    WidgetSerializer serializer( m_guiController, m_view->GetRenderDevice(), m_view->GetSpriteAtlasManager() );
    widget = serializer.ReadIn( *stream );
    if( widget != NULL )
    {
      m_widgets.push_back( widget );
    }
  }
  
  return widget;
}


void
BaseScreen::Enter( const std::vector<std::string>& args )
///
/// Callback function which is invoked when this State is first pushed onto the top of the stack. 
/// It must initializes all of the required subsystems needed for this state to work correctly.
///
/// @param caller
///   State controller that called this function.
///
/// @return
///   Nothing.
///
{  
  SetupScreenFromDefaultParameters();
     
  OnEnter();
}


void 
BaseScreen::Exit()
///
/// Callback function that is invoked when transitioning away from this screen.  Should 
/// deallocate any resources that are related exclusively to this screen.
///
/// @return
///   Nothing.
///
{
  Clear();
}


void 
BaseScreen::Update( float deltaTimeStep )
///
/// Main execution loop.
///
/// @param deltaTimeStep
///   Amount of elapsed time between calls.
///
/// @return
///   Nothing.
///
{
  ProcessDispatcher( m_dispatcher );
  OnUpdate( deltaTimeStep );
}


void 
BaseScreen::AddEvent( Event event )
///
/// Will add an event to this states dispatcher for later processing.
///
/// @param event
///   The Event to add.
///   
/// @return
///   Nothing.
///   
{
  m_dispatcher.AddEvent( event );
}


void 
BaseScreen::OnInit()
{

}


void 
BaseScreen::OnEnter()
///
/// On screen enter hook for subclasses.
///
/// @return
///   Nothing.
///
{}


void 
BaseScreen::OnExit() 
///
/// On screen exit hook for subclasses.
///
/// @return
///   Nothing.
///
{}


bool  
BaseScreen::OnKeyPressed( KeyCode keyCode, Flags<ModKey> modKeys ) 
///
/// Key press hook.
///
/// @return
///   Nothing.
///
{
  return m_guiController->HandleEvents( GuiManager::KEYPRESS, keyCode, 0, modKeys, Point(0,0), Point(0,0) );
}


bool  
BaseScreen::OnKeyReleased(KeyCode keyCode, Flags<ModKey> modKeys)
///
/// Key release hook.
///
/// @return
///   Nothing.
///
{
#ifndef NE_RETAIL
  // When in debug mode reset the profiler internal count variables if user presses the Backquote key.
  if( keyCode == Nebulae::VKC_BACKQUOTE )
  {
    Nebulae::ResetProfiler();
  }
#endif

  return m_guiController->HandleEvents( GuiManager::KEYRELEASE, keyCode,
    0, modKeys, Point(0,0), Point(0,0) );
}


bool 
BaseScreen::OnTouchDown(int32 x, int32 y, Flags<ModKey> modKeys)
///
/// Touch down hook.
///
/// @return
///   Nothing.
///
{
  return m_guiController->HandleEvents( GuiManager::TOUCHDOWN, VKC_UNKNOWN,
    0, modKeys, Point(x,y), Point(0,0) );
}


bool 
BaseScreen::OnTouchUp(int32 x, int32 y, Flags<ModKey> modKeys)
///
/// Touch up hook.
///
/// @return
///   Nothing.
///
{
  return m_guiController->HandleEvents( GuiManager::TOUCHUP, VKC_UNKNOWN,
    0, modKeys, Point(x,y), Point(0,0) );
}


bool 
BaseScreen::OnTouchDrag(int32 x, int32 y, Flags<ModKey> modKeys)
///
/// Touch drag hook.
///
/// @return
///   Nothing.
///
{
  return m_guiController->HandleEvents( GuiManager::TOUCHDRAG, VKC_UNKNOWN,
    0, modKeys, Point(x,y), Point() );
}


void
BaseScreen::OnUpdate( float elapsed )
///
/// Virtual Update implementation function.  To be overriden by any sub classes of BaseScreen
/// if they need to do any per frame updating.
///
/// @return
///   Nothing.
///
{}



struct BaseScreen::BaseScreenEventResolver : public Dispatcher::EventResolver
{
public:
  BaseScreen* m_screen;
  
public:
  BaseScreenEventResolver( BaseScreen* screen ) : m_screen(screen) {}
  
  /** Accepts an event argument and attempts to resolve it.  If the function deems that the
   event has been successfully resolved it should return true so that the event is removed
   from the queue. */
  virtual bool ResolveEvent( Event& event ) {
    if( m_screen == NULL ) return false;
    
    ScreenController* controller = m_screen->GetOwner();
    
    if( event.type == "_RELOAD_" ) {
      m_screen->Clear();
      m_screen->LoadScene();
      m_screen->LoadUi();
      
      return true;
    } else if( controller && controller->HasEdge(event.context) ) {
      std::vector<std::string> args;
      return m_screen->Goto( event.context, args );
    } else {
      return m_screen->InvokeAction( event.context );
    }
    
    return false;
  }
  
};


void 
BaseScreen::SetupScreenFromDefaultParameters()
{
  boost::any sceneValue;
  if( m_properties.Find("scene", sceneValue) )
  {    
    std::string sceneFile = boost::any_cast<std::string>( sceneValue );  
    LoadScene( sceneFile.c_str() );
  }

  boost::any layoutValue;
  if( m_properties.Find("gui_layout", layoutValue) )
  {
    // Layout can be either a single string or an array of strings.  Check the
    // variant type.
    bool isString = layoutValue.type() == typeid(std::string);
    bool isArray  = layoutValue.type() == typeid(std::vector<std::string>);
    
    if( isString )
    {
      std::string filename = boost::any_cast<std::string >(layoutValue);
      LoadUi( filename.c_str() );
    }
    else if( isArray )
    {
      std::vector<std::string> values = boost::any_cast<std::vector<std::string> >(layoutValue);

      std::for_each( values.begin(), values.end(), [&](const std::string& filename){
        LoadUi( filename.c_str() );
      });
    }
    else
    {
      NE_LOG( "Unknown type found." );
    }
  }
}


void
BaseScreen::ProcessDispatcher( Dispatcher& dispatcher )
///
/// virtual function used to process any user interactions with the UI.
///
/// @return
///   Nothing.
///
{
  BaseScreenEventResolver resolver(this);
  dispatcher.DispatchQueue( resolver );
}


bool
BaseScreen::InvokeAction( const std::string& context )
///
/// The game is requesting a change in state.
///
/// @param context
///   The change requested.
///
/// @return
///   Nothing.
///
{
  return false;
}


std::shared_ptr<Model>           
BaseScreen::GetModel()
{ return m_model; }


std::shared_ptr<View>            
BaseScreen::GetView()
{ return m_view; }


std::shared_ptr<AudioController>
BaseScreen::GetAudioController()
{ return m_audioController; }


BaseScreen::GuiControllerPointer
BaseScreen::GetGuiController()
{ return m_guiController; }


PropertyArray&                      
BaseScreen::GetProperties()
{ return m_properties; }
