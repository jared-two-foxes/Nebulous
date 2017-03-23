
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/Gui/GuiManager.h>

#include <States/BaseScreen.h>
#include <Model/Model.h>
#include <Model/Systems/AudioController.h>
#include <Controller/Dispatcher.h>
#include <View/View.h>

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class BaseScreenFixture : public ::testing::Test {
  protected:
    std::shared_ptr<Platform >         m_platform;
    std::shared_ptr<Model >            m_model;
    std::shared_ptr<View >             m_view;
    std::shared_ptr<ScreenController > m_controller;
    BaseScreen*                        m_baseScreen;
    Dispatcher                         m_dispatcher;

  protected:
    virtual void SetUp() 
    {
      m_platform = std::shared_ptr<Platform>( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr         fileSystem   = m_platform->GetFileSystem();
      Platform::WindowPtr             window       = m_platform->CreateApplicationWindow(0,0,800,600);
      Platform::RenderSystemPtr       renderDevice = m_platform->CreateRenderSystem( window );

      fileSystem->Mount( "disk", new DiskFileDevice("../UncompressedAssets") );
      File*                           is           = fileSystem->Open( "disk", "entityTemplates.json" );
      std::shared_ptr<TemplateCache > templateCache( new TemplateCache() );
      if( is != NULL ) 
      {
        templateCache->Load( *is );
      }

      m_model = std::shared_ptr<Model >( new Model(templateCache) );
      m_view  = std::shared_ptr<View >(  new View(fileSystem, renderDevice) );

      m_model->Init();
      m_view->Init( m_model );

      Nebulae::AudioManager*            audioManager = new Nebulae::AudioManager( fileSystem );
      std::shared_ptr<AudioController > audio( new AudioController(*audioManager) );
      std::shared_ptr<GuiManager >      guiManager( new GuiManager(fileSystem,renderDevice) );

      m_controller = std::shared_ptr<ScreenController >( new ScreenController() );
      m_baseScreen = new BaseScreen("TitleScreen", m_controller.get(), fileSystem, m_dispatcher, audio, guiManager, m_model, m_view);
    };

    virtual void TearDown() 
    {
      m_controller->Clear();
    }

};

TEST_F(BaseScreenFixture, LoadShouldAcceptAFilename) 
{
  //act
  bool result = m_baseScreen->Load( "level/TitleScreen.json" );

  //assert
  ASSERT_TRUE( result );
}