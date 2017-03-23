
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Profile/Profile.h>

#include "ScreenController.h"
#include "States/OptionScreen.h"

#include "Model/Model.h"
#include <Model/Systems/AudioController.h>
#include "Model/Systems/MotionSystem.h"

#include "View/View.h"

#include "Controller/Dispatcher.h"

#include "ViewCtrl/OptionScreenViewController.h"

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class OptionScreenTestFixture : public ::testing::Test 
{
  protected:
    std::shared_ptr<Platform >           m_platform;
    Nebulae::AudioManager*               m_audioManager;
    std::shared_ptr<Model >              m_model;
    std::shared_ptr<View >               m_view;
    std::shared_ptr<ScreenController >   m_controller;
    OptionScreen::ViewController*        m_viewController;
    OptionScreen*                        m_screen;
    Dispatcher                           m_dispatcher;

  protected:
    virtual void SetUp() 
    {
      m_platform = std::shared_ptr<Platform >( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr         fileSystem   = m_platform->GetFileSystem();
      Platform::WindowPtr             window       = m_platform->CreateApplicationWindow(0,0,800,600);
      Platform::RenderSystemPtr       renderDevice = m_platform->CreateRenderSystem( window );
      
      fileSystem->Mount( "disk", new DiskFileDevice("../UncompressedAssets") );
      File*                           is           = fileSystem->Open( "disk", "entityTemplates.json" );
      std::shared_ptr<TemplateCache > templateCache(new TemplateCache());
      
      if( is != NULL ) 
      {
        templateCache->Load( *is );
      }

      m_model = std::shared_ptr<Model >( new Model(templateCache) );
      m_view  = std::shared_ptr<View >( new View(fileSystem, renderDevice) );

      m_model->Init();
      m_view->Init( m_model );

      m_audioManager = new Nebulae::AudioManager( fileSystem );
      std::shared_ptr<AudioController > audio( new AudioController(*m_audioManager) );
      std::shared_ptr<GuiManager >      guiManager( new GuiManager(fileSystem,renderDevice) );

      m_controller = std::shared_ptr<ScreenController>( new ScreenController() );

      //m_audioManager->Initiate();
      m_viewController = new OptionScreen::ViewController( *m_controller );

      m_screen = new OptionScreen( m_controller.get(), fileSystem, m_dispatcher, audio, guiManager, m_model, m_view, m_viewController );

      std::vector<std::string> args;
      m_controller->AddScreen( m_screen );
      ScreenHandle sh = m_controller->CurrentScreen( "OptionsScreen" );
      sh.Enter( args );
    };

    virtual void TearDown() 
    {
      delete m_screen;         m_screen = nullptr;
      delete m_viewController; m_viewController = nullptr;
      delete m_audioManager;   m_audioManager = nullptr;
    }
};

TEST_F(OptionScreenTestFixture, SetState_StateSetToMenu_HasOptionButton) 
{
  //assert
  const BaseScreen::GuiControllerPointer model_gui = m_screen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("btnBack"); 
  ASSERT_NE( nullptr, widget );
}

//TEST_F(OptionScreenTestFixture, SetState_StateSetToMenu_OptionButtonCallbackIsValid) 
//{
//  //arrange
//  const BaseScreen::GuiControllerPointer model_gui = m_screen->GetGuiController();
//  const Button*                          button    = static_cast<Button* >( model_gui->Find("btnBack") ); 
//  EXPECT_NE( nullptr, button );
//  EXPECT_TRUE( !button->ClickedSignal.empty() ); 
//
//  //assert
//  boost::signals2::scoped_connection connection = button->ClickedSignal.connect( std::bind(&OptionScreen::ViewController::GotoGameState, m_viewController) );
//  ASSERT_EQ( connection, m_viewController->m_backButtonConnection );
//}
