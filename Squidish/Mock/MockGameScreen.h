#ifndef __SQUIDISH_TEST_MOCKGAMESCREEN_H__ 
#define __SQUIDISH_TEST_MOCKGAMESCREEN_H__ 

#include <States/GameScreen.h>

#include "gmock/gmock.h"

namespace Game {

class MockGameScreen : public GameScreen
{
  public:
    MockGameScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
      const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
      const ModelPointer& model, const ViewPointer& view, ViewController* viewController )
    : GameScreen(owner, fileSystem, dispatcher, audioController, guiMgr, model, view, viewController)
    {}
    
    MOCK_METHOD1( SetState, void (State state) );
    //MOCK_METHOD2( PlayAnimation, void (Entity& entity, const std::string& animation) );
    MOCK_METHOD0( Restart, void () );
    MOCK_METHOD0( Continue, void () );

};

}

#endif // __SQUIDISH_TEST_MOCKGAMESCREEN_H__ 
