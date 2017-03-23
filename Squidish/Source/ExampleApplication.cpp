
#include "ExampleApplication.h"
#include "ScreenController.h"
#include "ApplicationInputListener.h"

#include <States/GameScreen.h>
#include <States/OptionScreen.h>
#include <States/PauseScreen.h>

#include <Model/Model.h>
#include <Model/GameInputListener.h>
#include <Model/Systems/AudioController.h>

#include <View/View.h>

#include <ViewCtrl/GameScreenViewController.h>
#include <ViewCtrl/OptionScreenViewController.h>

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>
#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Beta/Beta.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/WidgetSerializer.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>


static const char* DEFAULT_ENTITY_TEMPLATE_PATH = "entityTemplates.json";

using namespace Game;
using namespace Nebulae;

ExampleApplication::ExampleApplication()
: Application()
{
#ifndef NE_RETAIL
  m_show_fps = true;
  m_show_profile_dump = false;
#endif
} 

ExampleApplication::~ExampleApplication()
{}

void 
ExampleApplication::Init()
{
  Initiate( 640, 960 );        //iphone4
  //Initiate( 640, 1136 );       //iphone5
  //Initiate( 750, 1334 );       //iphone6


  //Initiate( 800, 1280 );       //Galaxy Note
  //Initiate( 720, 1280 );       //Galaxy Note II
  //Initiate( 480, 800 );        //Galaxy S I, II, III
  //Initiate( 600, 1024 ); 

  Nebulae::Platform::FileSystemPtr fileSystem = GetPlatform()->GetFileSystem();

#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  fileSystem->Mount( "disk", new DiskFileDevice("..\\..\\UncompressedAssets") );
#else
  fileSystem->Mount( "zip", new ZipFileDevice("..\\..\\Assets", "Assets") );
#endif

  // Create the template object and pass it to the model's constructor!.
  std::shared_ptr<TemplateCache> templateCache = std::shared_ptr<TemplateCache>( new TemplateCache() );  
  const char* path = DEFAULT_ENTITY_TEMPLATE_PATH; 
  File* is = fileSystem->Open( SQUID_DEFAULT_ROOTDEVICE, path );
  if( is != NULL ) 
  {
    templateCache->Load( *is );
  }

  m_model = std::shared_ptr<Model>( new Model(templateCache) );
  m_model->Init();

  m_view = std::shared_ptr<View>( new View(m_pPlatform->GetFileSystem(), m_pRenderSystem) );
  m_view->Init( m_model );
  
  //@todo convert the audioManager to a shared_ptr too.
  Nebulae::AudioManager* audioManager = new Nebulae::AudioManager( fileSystem );
  audioManager->Initiate();
  m_audioController = std::shared_ptr<AudioController>( new AudioController(*audioManager) );

  m_guiController = std::shared_ptr<Nebulae::GuiManager>( new Nebulae::GuiManager(fileSystem, m_pRenderSystem) );

#ifndef NE_RETAIL
  LoadWidget( "gui/debug_stats.json");

  m_fps_label     = static_cast<Nebulae::TextControl*>(m_guiController->Find("fps"));
  m_profile_label = static_cast<Nebulae::TextControl*>(m_guiController->Find("profiler_dump"));
#endif

  std::vector<std::string> args;
  m_controller = std::shared_ptr<ScreenController >( new ScreenController() );
  m_controller->AddScreen( new GameScreen(m_controller.get(), fileSystem, m_dispatcher, m_audioController, m_guiController, m_model, m_view, new GameScreen::ViewController()) );
  m_controller->AddScreen( new OptionScreen(m_controller.get(), fileSystem, m_dispatcher, m_audioController, m_guiController, m_model, m_view, new OptionScreen::ViewController(*m_controller)) );
  m_controller->AddScreen( new PauseScreen(m_controller.get(), fileSystem, m_dispatcher, m_audioController, m_guiController, m_model, m_view) );
  m_controller->AddEdge( "GameScreen", "OptionScreen", "_GAME_TO_OPTIONS_", "" );
  m_controller->AddEdge( "GameScreen", "PauseScreen", "_GAME_TO_PAUSE_", "" );
  m_controller->AddEdge( "OptionScreen", "GameScreen", "_OPTIONS_TO_GAME_", "" );
  m_controller->AddEdge( "PauseScreen", "GameScreen", "_PAUSE_TO_GAME_", "" );
  ScreenHandle sh = m_controller->CurrentScreen( "GameScreen" );
  sh.Enter( args );

  ApplicationInputListener* app_listener = new ApplicationInputListener( *this );
  AddInputListener( app_listener );
  GameInputListener* listener = new GameInputListener( *m_controller, *m_model );
  AddInputListener( listener );
}

void 
ExampleApplication::ProcessSimulation( uint64 elapsed )
{
  PROFILE;

  // Basically need to do a conversion between microseconds and seconds as 
  // the subsystems are expecting time in seconds.  Should possibly change all
  // subsystems to also expect microseconds.
  float elapsedInSeconds = elapsed / 1000000.0f;

  if( m_controller->CurrentScreen() ) {
    m_controller->CurrentScreen().Update( elapsedInSeconds );
  }

  m_audioController->Update( elapsedInSeconds );
  
  m_view->Update( elapsed );
}

void 
ExampleApplication::ProcessFrame( uint64 elapsed )
{  
  {
    PROFILE;

    // Update the render state.
    m_view->ProcessAndRender( m_model );

    // Render Frame.
    m_view->Render();
    m_view->ProcessAndRender( m_guiController );
  }

#ifndef NE_RETAIL
  // Update the frames per second
  if( m_show_fps )
  {
    NE_ASSERT( m_fps_label != nullptr, "FPS label is not initiated" )();
    
    //@todo - if showing fps
    std::stringstream ss;
    ss << "FPS: " << std::fixed << std::setprecision(2) << m_framesPerSecond;
    m_fps_label->SetText( ss.str() );
  }

  if( m_show_profile_dump )
  {
    NE_ASSERT( m_profile_label != nullptr, "Profile label is not initated" )();
    
    //@todo - else if showing profile data
    std::string profile_dump;
    Nebulae::WriteProfileDataToBuffer( profile_dump );
    m_profile_label->SetText( profile_dump.c_str() );
  }
#endif
}


Widget*
ExampleApplication::LoadWidget( const char* filename )
{
  NE_ASSERT( filename, "filename is not specified" )( filename );

  if( filename == NULL || strlen(filename) == 0 ) 
  {
    return NULL;
  }

  Widget* widget = NULL;
  File*   stream = GetPlatform()->GetFileSystem()->Open( SQUID_DEFAULT_ROOTDEVICE, filename );
  if( stream != NULL ) 
  {
    WidgetSerializer serializer( m_guiController, m_pRenderSystem, m_view->GetSpriteAtlasManager() );
    widget = serializer.ReadIn( *stream );
  }
  
  return widget;
}

#ifndef NE_RETAIL
void ExampleApplication::ToggleFPS()
{
  NE_ASSERT( m_fps_label != nullptr, "FPS label is not initiated" )();

  m_show_fps = !m_show_fps;
  m_show_fps ? m_fps_label->Show() : m_fps_label->Hide();
}

void ExampleApplication::ToggleProfiler()
{
  NE_ASSERT( m_profile_label != nullptr, "Profile label is not initiated" )();
  
  m_show_profile_dump = !m_show_profile_dump;   
  m_show_profile_dump ? m_profile_label->Show() : m_profile_label->Hide();
}
#endif