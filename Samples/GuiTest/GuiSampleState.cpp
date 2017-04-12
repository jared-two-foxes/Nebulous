
#include "GuiSampleState.h"
#include "GuiInputListener.h"

#include <boost/bind.hpp>

#include <Nebulae/Common/Platform/Platform.h>
#include <Nebulae/Common/Window/Window.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Font/Font.h>
#include <Nebulae/Beta/Font/FontManager.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/WidgetRenderer.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/Gui/Control/StaticGraphic.h>
#include <Nebulae/Beta/StateStack/StateStack.h>


using namespace Nebulae;

void PlayButtonPressed( void* pOwner )
{
  printf( "%s", "The play button was pressed!\n" );
}

void HighScoreButtonPressed( void* pOwner )
{
  printf( "%s", "The high score button was pressed!\n" );
}

void OptionsButtonPressed( void* pOwner )
{
  printf( "%s", "The options button was pressed!\n" );
}

void AboutButtonPressed( void* pOwner )
{
  printf( "%s", "The about button was pressed!\n" );
}


GuiSampleState::GuiSampleState( const std::string& name ) 
: State( name ), 
  m_pCamera( NULL ), 
  m_pGuiManager( NULL ),
  m_inputListener( NULL ),
  m_fontMgr( NULL )
{}

GuiSampleState::~GuiSampleState()
{}

void 
GuiSampleState::SetupInternalSystems( StateStack* caller )
{
  // Grab Application variables to help with setup.
  m_pRenderSystem = caller->GetRenderSystem();
  
  // Setup camera
  m_pCamera = new Camera();
  Vector4 vCameraEye( 0.0f, 0.0f, 5.0f );
  Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
  Vector4 vUp( 0.0f, 1.0f, 0.0f );
  m_pCamera->LookAt( vCameraEye, vLookAt, vUp );
  m_pCamera->SetOrtho( 640.0f, 960.0f, 0, 1.0f );

  // Setup Gui Subsystems.
  m_fontMgr       = new FontManager(); 
  m_pGuiManager   = new GuiManager( caller->GetPlatform()->GetFileSystem(), m_pRenderSystem );
  m_inputListener = new GuiInputListener( *m_pGuiManager );
  m_renderer      = new WidgetRenderer( caller->GetPlatform()->GetFileSystem(), nullptr, m_pRenderSystem );
}

void 
GuiSampleState::SetupGuiComponents( StateStack* caller )
{
  // Should be created and passed to the GuiManager on initiation?
  Texture*    pPlayTexture         = m_pRenderSystem->CreateTexture( "menu_btn_play.png" );
  Texture*    pAboutTexture        = m_pRenderSystem->CreateTexture( "menu_btn_about.png" );
  Texture*    pHighScoreTexture    = m_pRenderSystem->CreateTexture( "menu_btn_hiS.png" );
  Texture*    pOptionsTexture      = m_pRenderSystem->CreateTexture( "menu_btn_options.png" );
  SubTexture* pPlaySubTexture      = new SubTexture( pPlayTexture, 0, 0, 1.0f, 1.0f );
  SubTexture* pAboutSubTexture     = new SubTexture( pAboutTexture, 0, 0, 1.0f, 1.0f );
  SubTexture* pHighScoreSubTexture = new SubTexture( pHighScoreTexture, 0, 0, 1.0f, 1.0f );
  SubTexture* pOptionsSubTexture   = new SubTexture( pOptionsTexture, 0, 0, 1.0f, 1.0f );

  const std::shared_ptr<Nebulae::WidgetFactory> factory = m_pGuiManager->GetWidgetFactory();

  Widget* pWidget = new Control( *factory, 0, 0, 960, 640 );

  {
    ButtonControl* pButton = factory->CreateButton( 0, 0, 364, 100, "", std::shared_ptr<Font>(), CLR_BLACK );
    pButton->ClickedSignal.connect( boost::bind(&PlayButtonPressed, this) );
    pButton->SetUnpressedGraphic( *pPlaySubTexture );
    pButton->SetPressedGraphic( *pAboutSubTexture );
    pWidget->AttachChild( pButton );
  }

  {
    ButtonControl* pButton = factory->CreateButton( 0, 100, 364, 100, "", std::shared_ptr<Font>(), CLR_BLACK );
    pButton->ClickedSignal.connect( boost::bind(&AboutButtonPressed, this) );
    pButton->SetUnpressedGraphic( *pAboutSubTexture );
    pButton->SetPressedGraphic( *pPlaySubTexture );
    pWidget->AttachChild( pButton );
  }

  {
    ButtonControl* pButton = factory->CreateButton( 0, 200, 364, 94, "", std::shared_ptr<Font>(), CLR_BLACK );
    pButton->ClickedSignal.connect( boost::bind(&HighScoreButtonPressed, this) );
    pButton->SetUnpressedGraphic( *pHighScoreSubTexture );
    pButton->SetPressedGraphic( *pAboutSubTexture );
    pWidget->AttachChild( pButton );
  }

  {
    ButtonControl* pButton = factory->CreateButton( 0, 300, 314, 84, "", std::shared_ptr<Font>(), CLR_BLACK );
    pButton->ClickedSignal.connect( boost::bind(&OptionsButtonPressed, this) );
    pButton->SetUnpressedGraphic( *pOptionsSubTexture );
    pButton->SetPressedGraphic( *pAboutSubTexture );
    pWidget->AttachChild( pButton );
  }

  m_pGuiManager->Register( pWidget );
  
  
//
// Setup text labels.
// 
  std::shared_ptr<Font> arialPtr    = m_pGuiManager->GetFont( "fonts/arial.ttf", 24 );
  std::shared_ptr<Font> groboldPtr  = m_pGuiManager->GetFont( "fonts/grobold.ttf", 24 );
  std::shared_ptr<Font> celestiaPtr = m_pGuiManager->GetFont( "fonts/celestia redux.ttf", 24 );

  TextControl* pLabel1  = factory->CreateTextControl( 0, 600, 200, 100, "The quick brown fox jumps over the lazy dog!", arialPtr, CLR_YELLOW, FORMAT_TOP | FORMAT_LEFT | FORMAT_WORDBREAK );
  TextControl* pLabel2  = factory->CreateTextControl( 440, 600, 200, 100, "The quick brown fox jumps over the lazy dog!", groboldPtr, CLR_BLUE, FORMAT_TOP | FORMAT_RIGHT | FORMAT_WORDBREAK );
  TextControl* pLabel3  = factory->CreateTextControl( 220, 700, 200, 100, "The quick brown fox jumps over the lazy dog!", celestiaPtr, CLR_WHITE, FORMAT_BOTTOM | FORMAT_CENTER | FORMAT_WORDBREAK );
  
  m_pGuiManager->Register( pLabel1 );
  m_pGuiManager->Register( pLabel2 );
  m_pGuiManager->Register( pLabel3 );
  

//
// Attach GuiManager to the main window listener.
// 
  caller->AddInputListener( m_inputListener );
}

void 
GuiSampleState::Enter( StateStack* caller )
{
  SetupInternalSystems( caller );
  SetupGuiComponents( caller );
}

void 
GuiSampleState::Exit( StateStack* caller )
{
  if( m_pGuiManager ) {
    delete m_pGuiManager;
  }
  m_pGuiManager = NULL;

  if( m_inputListener ) {
    delete m_inputListener;
  }
  m_inputListener = NULL;

  if( m_fontMgr ) {
    delete m_fontMgr;
  }
  m_fontMgr = NULL;

  if( m_pCamera ) {
    delete m_pCamera;
  }
  m_pCamera = NULL;
}

void 
GuiSampleState::Update( float fDeltaTimeStep, StateStack* pCaller )
{
} 

void 
GuiSampleState::Render() const
{
  if( m_renderer != NULL ) {
    m_renderer->Render( m_pGuiManager->GetRegisteredWidgets() );
  }
}