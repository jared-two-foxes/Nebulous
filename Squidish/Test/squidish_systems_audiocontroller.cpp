
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include <ScreenController.h>
#include <Model/Systems/AudioController.h>
#include <View/View.h>

#include <../../../tests/Audio/MockAudioManager.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace Nebulae;
using namespace Game;


class AudioControllerTestFixture : public ::testing::Test 
{
protected:
  boost::shared_ptr<Platform >         m_platform;
  boost::shared_ptr<MockAudioManager > m_audioManager;
  boost::shared_ptr<AudioController >  m_controller;

  protected:
    virtual void SetUp() 
    {      
      using ::testing::NiceMock;

      m_platform = boost::shared_ptr<Platform>( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr fileSystem = m_platform->GetFileSystem();
      fileSystem->Mount( "disk", new DiskFileDevice("../UncompressedAssets") );

      m_audioManager = boost::shared_ptr<Nebulae::MockAudioManager>( new NiceMock<MockAudioManager>(fileSystem) );
      m_audioManager->Initiate();

      m_controller   = boost::shared_ptr<AudioController>( new AudioController(*m_audioManager.get()) );
    };

    virtual void TearDown() 
    {}

};

TEST_F(AudioControllerTestFixture, Destructor_ControllerContainsValidHandles_ShouldUnloadAllSounds) 
{
  using ::testing::_;
  using ::testing::Invoke;

  ON_CALL( *m_audioManager, CreateSound(_))
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateSound) );
  ON_CALL( *m_audioManager, CreateEmitter())
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateEmitter) );

  //arrange
  m_controller->PlayMusic( "music", "sounds//ss_01_01.ogg" );

  EXPECT_CALL( *m_audioManager, Stop(_) )
    .Times(1);

  //act
  m_controller = nullptr;
}

TEST_F(AudioControllerTestFixture, PlayMusic_NullStringForFilename_AddsNoActiveEmitter) 
{
  //arrange
  std::string identifier = "";

  //act
  m_controller->PlayMusic( identifier.c_str(), nullptr );
  
  //assert
  ASSERT_TRUE( !m_controller->IsNameValid(identifier.c_str()) );
}

TEST_F(AudioControllerTestFixture, PlayMusic_EmptyStringForFilename_AddsNoActiveEmitter) 
{
  //arrange
  std::string identifier = "";
  std::string filename   = "";

  //act
  m_controller->PlayMusic( identifier.c_str(), filename.c_str() );
  
  //assert
  ASSERT_TRUE( !m_controller->IsNameValid(identifier.c_str()) );
}

TEST_F(AudioControllerTestFixture, PlayMusic_ValidIdentifier_CreatesANamedEmitter) 
{
  using ::testing::_;
  using ::testing::Invoke;

  //arrange
  std::string identifier = "music";
  std::string filename   = "sounds//ss_01_01.ogg";
  
  ON_CALL( *m_audioManager, CreateSound(_))
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateSound) );
  ON_CALL( *m_audioManager, CreateEmitter())
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateEmitter) );

  //act
  m_controller->PlayMusic( identifier.c_str(), filename.c_str() );
  
  //assert
  ASSERT_TRUE( m_controller->IsNameValid(identifier.c_str()) );
}

TEST_F(AudioControllerTestFixture, PauseMusic_ValidIdentifier_ShouldPauseNamedEmitter) 
{
  using ::testing::_;
  using ::testing::Invoke;

  //arrange
  std::string identifier = "music";
  std::string filename   = "sounds//ss_01_01.ogg";

  ON_CALL( *m_audioManager, CreateSound(_))
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateSound) );
  ON_CALL( *m_audioManager, CreateEmitter())
    .WillByDefault( Invoke(m_audioManager.get(), &MockAudioManager::ConcreteCreateEmitter) );

  m_controller->PlayMusic( identifier.c_str(), filename.c_str() );

  EXPECT_CALL( *m_audioManager, Pause(_) )
    .Times(1);

  //act
  m_controller->PauseMusic( identifier.c_str() );
  
  //assert
  ASSERT_TRUE( m_controller->IsPaused(identifier.c_str()) );
}