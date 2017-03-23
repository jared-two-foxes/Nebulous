
#include "GameScreenViewController.h"

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>

using namespace Game;

GameScreen::ViewController::ViewController()
: m_screen(nullptr),
  m_scoreControl(nullptr),
  m_currentScore(-1)
{}

void 
GameScreen::ViewController::SetScreen( GameScreen* screen )
{
  m_screen = screen;
}

void 
GameScreen::ViewController::UpdateScore( int score )
{
  if( m_scoreControl != nullptr && m_currentScore != score ) 
  {
    m_currentScore = score;
    
    char  scoreBuffer[16];
    int32 count = sprintf_s( &scoreBuffer[0], 16, "%d", m_currentScore );
    scoreBuffer[count+1] = '\0';

    m_scoreControl->SetText( &scoreBuffer[0] );
  }
}

void
GameScreen::ViewController::Init( BaseScreen::GuiControllerPointer gui )
{
  //@todo Move into the loading procedure & file-format?
  
  m_scoreControl = static_cast<Nebulae::TextControl* >(gui->Find("txt_score"));
  if( m_scoreControl != nullptr ) 
  {    
    m_scoreControl->SetTextFormat( Nebulae::FORMAT_RIGHT );
  }

  Nebulae::ButtonControl* btnPlay = static_cast<Nebulae::ButtonControl* >(gui->Find("btnPlay"));
  if( btnPlay != NULL )
  {
    btnPlay->ClickedSignal.connect( std::bind(&GameScreen::ViewController::StartGame, this) );
  }

  Nebulae::ButtonControl* btnOption = static_cast<Nebulae::ButtonControl* >(gui->Find("btnOptions"));
  if( btnOption != NULL )
  {
    btnOption->ClickedSignal.connect( std::bind(&GameScreen::ViewController::GotoOptions, this) );
  }
}

void
GameScreen::ViewController::StateDidChange( GameScreen::State state, BaseScreen::GuiControllerPointer gui )
{
  std::vector<std::string > visibleWidgets;
  std::vector<std::string > hiddenWidgets;

  if( state == STATE_MENU )
  {
    hiddenWidgets.push_back( "btn_pause" );
    hiddenWidgets.push_back( "game_over_panel" );
    visibleWidgets.push_back( "menu_panel" );
    hiddenWidgets.push_back( "txt_score" );
    visibleWidgets.push_back( "img_logo" );
  }
  else if( state == STATE_INTRO || state == STATE_ACTIVE )
  {
    visibleWidgets.push_back( "btn_pause" );
    hiddenWidgets.push_back( "game_over_panel" );
    hiddenWidgets.push_back( "menu_panel" );
    visibleWidgets.push_back( "txt_score" );
    hiddenWidgets.push_back( "img_logo" );
  }
  else if( state == STATE_DEAD )
  {
    hiddenWidgets.push_back( "btn_pause" );
    visibleWidgets.push_back( "game_over_panel" );
    hiddenWidgets.push_back( "menu_panel" );
    hiddenWidgets.push_back( "txt_score" );
    hiddenWidgets.push_back( "img_logo" );
  }
   
  SetupGUI( gui, visibleWidgets, hiddenWidgets );
}
 
void 
GameScreen::ViewController::StartGame()
{
  NE_ASSERT( m_screen->GetState() == STATE_MENU, "Expected game to be in the menu state" )();

  m_screen->SetState( STATE_INTRO );
}


void 
GameScreen::ViewController::GotoOptions()
{
  std::vector<std::string> args;
  m_screen->Goto( "_GAME_TO_OPTIONS_", args );
}

void 
GameScreen::ViewController::SetupGUI( BaseScreen::GuiControllerPointer gui, const std::vector<std::string > visibleWidgets, const std::vector<std::string > hiddenWidgets )
{
  std::vector<std::string >::const_iterator itr;
  
  // Show the visible widgets.
  for( itr = visibleWidgets.begin(); itr != visibleWidgets.end(); ++itr )
  {
    Nebulae::Widget* widget = gui->Find( (*itr).c_str() );
    if( widget != NULL )
    {
      widget->Show();
    }
  }

  // Hide any non visible widgets.
  for( itr = hiddenWidgets.begin(); itr != hiddenWidgets.end(); ++itr )
  {
    Nebulae::Widget* widget = gui->Find( (*itr).c_str() );
    if( widget != NULL )
    {
      widget->Hide();
    }
  }
}