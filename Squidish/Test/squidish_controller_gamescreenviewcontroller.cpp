
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Profile/Profile.h>

#include <States/GameScreen.h>

#include <Model/Model.h>
#include <Model/Systems/AudioController.h>
#include <Model/Systems/MotionSystem.h>

#include <Controller/Dispatcher.h>

#include <ViewCtrl/GameScreenViewController.h>

#include <View/View.h>

#include "../Mock/MockGameScreen.h"

#include <gtest/gtest.h>

using namespace Nebulae;
using namespace Game;


class GameScreenViewControllerFixture : public ::testing::Test 
{
protected:
  std::shared_ptr<Platform >           m_platform;
  std::shared_ptr<Model >              m_model;
  std::shared_ptr<View >               m_view;
  std::shared_ptr<AudioController >    m_audioController;
  std::shared_ptr<GuiManager >         m_guiManager;
  std::shared_ptr<AudioManager >       m_audioManager;
  Dispatcher                           m_dispatcher;
  MockGameScreen*                      m_mock_screen;

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

      m_audioManager = std::shared_ptr<AudioManager >( new Nebulae::AudioManager(fileSystem) );
      m_audioManager->Initiate();

      m_model           = std::shared_ptr<Model >( new Model(templateCache) );
      m_view            = std::shared_ptr<View >( new View(fileSystem, renderDevice) );
      m_audioController = std::shared_ptr<AudioController >( new AudioController(*m_audioManager) );
      m_guiManager      = std::shared_ptr<GuiManager >( new GuiManager(fileSystem,renderDevice) );

      m_model->Init();
      m_view->Init( m_model );

      m_mock_screen = new MockGameScreen( nullptr, fileSystem, m_dispatcher, m_audioController, m_guiManager, m_model, m_view, nullptr );
    };

    virtual void TearDown() 
    {
      delete m_mock_screen;
    }

};

TEST_F(GameScreenViewControllerFixture, StartGame_ValidScreen_ShouldBeginIntroPhase) 
{
  //arrange
  GameScreen::ViewController viewController;
  viewController.SetScreen( m_mock_screen );

  EXPECT_CALL( *m_mock_screen, SetState(GameScreen::STATE_INTRO) )
    .Times(1);

  //act
  viewController.StartGame();
}
