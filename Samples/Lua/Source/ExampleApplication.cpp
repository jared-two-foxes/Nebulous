
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
#include <Nebulae/Beta/SpriteBatch/SpriteBatch.h>

static const char* DEFAULT_ENTITY_TEMPLATE_PATH = "entityTemplates.json";

using namespace Nebulae;


void 
ExampleApplication::Init()
{
  Initiate( 800, 600 );

  Platform::FileSystemPtr fileSystem = GetPlatform()->GetFileSystem();

  // Setup the file system devices.
#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
  fileSystem->Mount( "disk", new DiskFileDevice("..//..//Samples//Lua//Assets") );
#else
  NE_ASSERT( false, "Zip device is not supported for example." )();
#endif

  // Setup Audio if included
#ifdef NEBULAE_AUDIO
  m_audioController = std::make_shared<Nebulae::AudioManager >(fileSystem);
  m_audioController->Initiate();
#endif

  // Setup script interpreter
  m_scriptInterpreter = std::make_shared<Nebulae::LuaInterpreter >();
  m_scriptInterpreter->Initialize();

  // Setup GUI system
  m_guiController     = std::make_shared<Nebulae::GuiManager >( fileSystem, m_pRenderSystem );

  // Setup Gui Renderer
#ifdef USE_BATCHER
  m_batcher = std::make_shared<SpriteBatch >( m_pRenderSystem );
  m_batcher->Init();
  m_widgetRenderer    = std::make_shared<Nebulae::WidgetRenderer >( GetPlatform()->GetFileSystem(), m_batcher.get(), m_pRenderSystem );
#else
  m_widgetRenderer = std::make_shared<Nebulae::WidgetRenderer >(GetPlatform()->GetFileSystem(), nullptr, m_pRenderSystem);
#endif
  m_widgetRenderer->Init();
  std::shared_ptr<Nebulae::Font > font = m_guiController->GetFont("fonts//consola.ttf", 12);
  m_widgetRenderer->AddFont(font);

  
  //@todo. Separate the WidgetFactory from the GuiManager.  WidgetFactory should be passed to the GuiManager 
  //       as a constructor argument, if at all!
  TextControl* consoleTextElement = m_guiController->GetWidgetFactory()->CreateTextControl( 0, 0, 800, 200, "Initial Line of text!", font, CLR_WHITE, FORMAT_TOP | FORMAT_LEFT );
  m_debugConsole = std::make_shared<Nebulae::DebugConsole >( m_scriptInterpreter, consoleTextElement, GetPlatform()->GetKeyboard() );
  m_debugConsole->AddLine( "This is a line I have added to the debug console!" );
  
  Texture*       texture     = m_pRenderSystem->CreateTexture( "textures//menu_btn_play.png" );
  SubTexture*    sub_texture = new SubTexture( texture, 0, 0, 1.0f, 1.0f );
  StaticGraphic* graphic     = m_guiController->GetWidgetFactory()->CreateStaticGraphic( 0, 300, 364, 100, sub_texture );
  
  m_guiController->Register( consoleTextElement );
  m_guiController->Register( graphic );

  AddInputListener( m_debugConsole.get() );
}


void 
ExampleApplication::ProcessSimulation( uint64 elapsed )
{
#ifdef NEBULAE_AUDIO
  // @todo - convert elapsed into a float elapsed.
  m_audioController->Update( 1.0f / 30 );
#endif 
}


void 
ExampleApplication::ProcessFrame( uint64 elapsed )
{
  m_debugConsole->PushToGui();
  m_widgetRenderer->Render( m_guiController->GetRegisteredWidgets() );
}
