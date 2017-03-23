
#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>
#include <Nebulae/Beta/Gui/GuiManager.h>

#include <ScreenController.h>
#include <States/GameScreen.h>
#include <Model/Model.h>
#include <Model/Systems/AudioController.h>
#include <View/View.h>

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class ScreenControllerTestFixture : public ::testing::Test {
  protected:
    std::shared_ptr<ScreenController > m_controller;

  protected:
    virtual void SetUp() {      
      m_controller = std::shared_ptr<ScreenController>( new ScreenController() );
    };

    virtual void TearDown() {
      m_controller->Clear();
    }

};


TEST_F(ScreenControllerTestFixture, ClearRemovesAllAddedScreens) {
  //arrange
  Screen* screen01 = new Screen( "screen_01", m_controller.get() );
  Screen* screen02 = new Screen( "screen_02", m_controller.get() );
  m_controller->AddScreen( screen01 );
  m_controller->AddScreen( screen02 );

  //act
  m_controller->Clear();

  //assert
  ASSERT_EQ( 0, m_controller->GetScreenCount() );
}

TEST_F(ScreenControllerTestFixture, ClearRemovesAllAddedEdges) {
  //arrange
  Screen* screen01 = new Screen( "screen_01", m_controller.get() );
  Screen* screen02 = new Screen( "screen_02", m_controller.get() );
  m_controller->AddScreen( screen01 );
  m_controller->AddScreen( screen02 );
  m_controller->AddEdge( "screen_01", "screen_02", "_condition_", "args0" );

  //act
  m_controller->Clear();

  //assert
  ASSERT_EQ( 0, m_controller->GetEdgeCount() );
}