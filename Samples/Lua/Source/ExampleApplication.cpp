
#include "ExampleApplication.h"

#include "Lua/LuaInterpreter.h"
#include "Widgets/DebugConsole.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/Platform/Platform.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>

#include <Nebulae/Beta/Beta.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/WidgetRenderer.h>
#include <Nebulae/Beta/Gui/Control/StaticGraphic.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>

static const char* DEFAULT_ENTITY_TEMPLATE_PATH = "entityTemplates.json";

using namespace Nebulae;


ExampleApplication::ExampleApplication()
: Application(),
  m_debugConsole(NULL)
{} 


ExampleApplication::~ExampleApplication()
{
  if( m_debugConsole != NULL )
  {
    delete m_debugConsole;
  }
  m_debugConsole = NULL;
}


void 
ExampleApplication::Init()
{
  Initiate( 800, 600 );

  Platform::FileSystemPtr fileSystem = GetPlatform()->GetFileSystem();

#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  fileSystem->Mount( "disk", new DiskFileDevice("..//..//Samples//Lua//Assets") );
#else
  NE_ASSERT( false, "Zip device is not supported for example." )();
#endif

  m_audioController   = boost::shared_ptr<Nebulae::AudioManager>( new AudioManager(fileSystem) );
  m_guiController     = boost::shared_ptr<Nebulae::GuiManager>( new GuiManager(fileSystem, m_pRenderSystem) );
  
  m_widgetRenderer    = new WidgetRenderer( GetPlatform()->GetFileSystem(), nullptr, m_pRenderSystem );
  m_widgetRenderer->Init();

  m_scriptInterpreter = boost::shared_ptr<Nebulae::LuaInterpreter>( new LuaInterpreter() );
  
  m_audioController->Initiate();
  m_scriptInterpreter->Initialize();
  
  std::shared_ptr<Nebulae::Font > font = m_guiController->GetFont( "fonts//consola.ttf", 12 );
  m_widgetRenderer->AddFont( font );

  //@todo. Separate the WidgetFactory from the GuiManager.  WidgetFactory should be passed to the GuiManager 
  //       as a constructor argument, if at all!
  TextControl* consoleTextElement = m_guiController->GetWidgetFactory()->CreateTextControl( 0, 0, 800, 200, "Initial Line of text!", font, CLR_WHITE, FORMAT_TOP | FORMAT_LEFT );
  m_debugConsole = new DebugConsole( m_scriptInterpreter, consoleTextElement, GetPlatform()->GetKeyboard() );
  m_debugConsole->AddLine( "This is a line I have added to the debug console!" );
  

  Texture*       texture     = m_pRenderSystem->CreateTexture( "textures//menu_btn_play.png" );
  SubTexture*    sub_texture = new SubTexture( texture, 0, 0, 1.0f, 1.0f );
  StaticGraphic* graphic     = m_guiController->GetWidgetFactory()->CreateStaticGraphic( 0, 300, 364, 100, sub_texture );
  
  m_guiController->Register( consoleTextElement );
  m_guiController->Register( graphic );

  AddInputListener( m_debugConsole );
}


void 
ExampleApplication::ProcessSimulation( uint64 elapsed )
{

}


void 
ExampleApplication::ProcessFrame( uint64 elapsed )
{
  m_audioController->Update( 1.0f / 30 );

  m_debugConsole->PushToGui();

  m_pRenderSystem->Clear();

  m_widgetRenderer->Render( m_guiController->GetRegisteredWidgets() );

  m_pRenderSystem->SwapBuffers();
}
