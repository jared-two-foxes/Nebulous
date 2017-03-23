
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Profile/Profile.h>

#include "ScreenController.h"
#include "States/GameScreen.h"

#include "Model/Model.h"
#include <Model/Systems/AudioController.h>
#include "Model/Systems/MotionSystem.h"

#include "View/View.h"

#include "Controller/Dispatcher.h"

#include "ViewCtrl/GameScreenViewController.h"


#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class GameScreenTestFixture : public ::testing::Test {
  protected:
    std::shared_ptr<Platform >           m_platform;
    Nebulae::AudioManager*               m_audioManager;
    std::shared_ptr<Model >              m_model;
    std::shared_ptr<View >               m_view;
    std::shared_ptr<ScreenController >   m_controller;
    GameScreen::ViewController*          m_viewController;
    GameScreen*                          m_gameScreen;
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

      m_audioManager->Initiate();
      m_viewController = new GameScreen::ViewController();

      m_controller = std::shared_ptr<ScreenController>( new ScreenController() );
      m_gameScreen = new GameScreen( m_controller.get(), fileSystem, m_dispatcher, audio, guiManager, m_model, m_view, m_viewController );

      std::vector<std::string> args;
      m_controller->AddScreen( m_gameScreen );
      ScreenHandle sh = m_controller->CurrentScreen( "GameScreen" );
      sh.Enter( args );
    };

    virtual void TearDown() 
    {
      delete m_gameScreen;     m_gameScreen = nullptr;
      delete m_viewController; m_viewController = nullptr;
      delete m_audioManager;   m_audioManager = nullptr;
    }
};


TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_HasPlayButton) 
{
  // arrange & act - do nothing.

  //assert
  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("btnPlay"); 
  ASSERT_NE( nullptr, widget );
}

//TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_PlayButtonCallbackIsValid) 
//{
//  // arrange
//  std::function<void ()> function( std::bind(&GameScreen::ViewController::StartGame, m_viewController) );
//  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
//  const Button*                          button    = static_cast<Button* >( model_gui->Find("btnPlay") ); 
//  EXPECT_NE( nullptr, button );
//  EXPECT_NE( nullptr, button->GetCallback() );
//  
//  //assert
//  ASSERT_EQ( function.target<void ()>(), button->GetCallback().target<void ()>() );
//}

TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_HasOptionButton) 
{
  //assert
  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("btnOptions"); 
  ASSERT_NE( nullptr, widget );
}

//TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_OptionButtonCallbackIsValid) 
//{
//  //arrange
//  std::function<void ()> function( std::bind(&GameScreen::ViewController::GotoOptions, m_viewController) );
//  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
//  const Button*                          button    = static_cast<Button* >( model_gui->Find("btnOptions") ); 
//  EXPECT_NE( nullptr, button );
//  EXPECT_NE( nullptr, button->GetCallback() );
//
//  //assert
//  ASSERT_EQ( function.target<void ()>(), button->GetCallback().target<void ()>() );
//}

TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_HasPauseButton) 
{
  // arrange & act - do nothing.

  //assert
  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("btn_pause"); 
  ASSERT_NE( nullptr, widget );
}

TEST_F(GameScreenTestFixture, SetState_StateSetToMenu_HasScoreTextField) 
{
  // arrange & act - do nothing.

  //act
  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("txt_score"); 
  ASSERT_NE( nullptr, widget );
}

TEST_F(GameScreenTestFixture, Setup_View_HasGameOverTextField) 
{
  // arrange & act - do nothing.
    
  //act 
  const BaseScreen::GuiControllerPointer model_gui = m_gameScreen->GetGuiController();
  const Widget*                          widget    = model_gui->Find("txt_gameover"); 
  ASSERT_NE( nullptr, widget );
}


TEST_F(GameScreenTestFixture, GameOverTextFieldTextShowsStringGameOver) 
{
  //act 
  TextControl* gameoverText = static_cast<TextControl*>(m_gameScreen->GetGuiController()->Find("txt_gameover"));

  //assert
  std::string expectedString("GAME OVER!"); //< this is incorrect, shouldnt test against a string.
  ASSERT_EQ( expectedString, gameoverText->GetText() );
}

TEST_F(GameScreenTestFixture, MenuState_GameOverTextField_ShouldNotBeVisible) 
{
  //act 

  //assert
  Widget* gameoverText = m_gameScreen->GetGuiController()->Find( "txt_gameover" );

  EXPECT_EQ( GameScreen::STATE_MENU, m_gameScreen->GetState() );
  ASSERT_TRUE( !gameoverText->IsVisible() );
}

TEST_F(GameScreenTestFixture, DeadState_GameOverTextField_ShouldBeVisible) 
{
  //act
  m_gameScreen->SetState( GameScreen::STATE_DEAD );

  //assert
  Widget* gameoverText = m_gameScreen->GetGuiController()->Find( "txt_gameover" );
  EXPECT_EQ( GameScreen::STATE_DEAD, m_gameScreen->GetState() );
  EXPECT_TRUE( gameoverText != NULL );
  ASSERT_TRUE( gameoverText->IsVisible() );
}

TEST_F(GameScreenTestFixture, MotionSystemShouldPauseWhileInDeadState) 
{
  //act
  m_gameScreen->SetState( GameScreen::STATE_DEAD );

  //assert
  const MotionSystem& motionSystem = m_gameScreen->GetMotionSystem();
  EXPECT_EQ( GameScreen::STATE_DEAD, m_gameScreen->GetState() );
  ASSERT_TRUE( !motionSystem.IsActive() );
}
