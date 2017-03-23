#ifndef __GAME_STATES_GAMESCREEN_VIEWCONTROLLER_H__ 
#define __GAME_STATES_GAMESCREEN_VIEWCONTROLLER_H__

#include <States/GameScreen.h>

namespace Game {

class GameScreen::ViewController
{
private:
  GameScreen*           m_screen;
  Nebulae::TextControl* m_scoreControl;
  int32                 m_currentScore;
  
  public:
    ViewController();

    void SetScreen( GameScreen* screen );
    void UpdateScore( int score );

    virtual void Init( BaseScreen::GuiControllerPointer gui );
    virtual void StateDidChange( GameScreen::State state, BaseScreen::GuiControllerPointer gui );

    virtual void StartGame();
    virtual void GotoOptions();

  private:
  	void SetupGUI( BaseScreen::GuiControllerPointer gui, const std::vector<std::string > visibleWidgets, const std::vector<std::string > hiddenWidgets );

};

} 

#endif // __GAME_STATES_GAMESCREEN_VIEWCONTROLLER_H__ 