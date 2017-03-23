
#include "OptionScreenViewController.h"

#include <ScreenController.h>

#include <View/View.h>

#include <Model/Systems/AudioController.h> //< this does not seem like the correct location for this...?

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Control/Slider.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>

using namespace Game;

OptionScreen::ViewController::ViewController( ScreenController& screenController )
: m_audioController(nullptr),
  m_screenController(screenController)
{}

void
OptionScreen::ViewController::SetAudioController( AudioControllerPointer audioController )
{
  m_audioController = audioController;
}

void
OptionScreen::ViewController::Setup( BaseScreen::RenderSystemPointer renderDevice, SpriteAtlasPtr atlasManagerPtr, BaseScreen::GuiControllerPointer gui )
{
  Nebulae::SpriteAtlas* atlas       = atlasManagerPtr->GetByName( "sprites/ui_atlas.json" );
  Nebulae::SubTexture*  sub_texture = atlas->FindModuleSubTexture( "opts_eyeBall" );
  Nebulae::Slider*      musicSlider = static_cast<Nebulae::Slider*>( gui->Find("music_slider") );
  if( musicSlider != nullptr ) 
  {
    Nebulae::ButtonControl* tab = musicSlider->GetTab();
    NE_ASSERT( tab, "Unable to find the 'tab' object for the music slider." )();
    tab->SetUnpressedGraphic( *sub_texture );
    tab->SetPressedGraphic( *sub_texture );
    
    musicSlider->SlidSignal.connect( boost::bind(&OptionScreen::ViewController::MusicVolumeSliderMoved, this, _1, _2, _3) );
    musicSlider->SlideTo( 1.0f );
  }
  
  Nebulae::Slider* sfxSlider = static_cast<Nebulae::Slider*>( gui->Find("sfx_slider") );
  if( sfxSlider != nullptr ) 
  {
    Nebulae::ButtonControl* tab = sfxSlider->GetTab();
    tab->SetUnpressedGraphic( *sub_texture );
    tab->SetPressedGraphic( *sub_texture );
  }
  
  Nebulae::ButtonControl* back_button = static_cast<Nebulae::ButtonControl*>( gui->Find("btnBack") );
  if( back_button != nullptr )
  {
    m_backButtonConnection = back_button->ClickedSignal.connect( std::bind(&OptionScreen::ViewController::GotoGameState, this) );
  }
}

void 
OptionScreen::ViewController::MusicVolumeSliderMoved( Real current, Real min, Real max )
{
  m_audioController->SetMasterVolume( current );
}

void 
OptionScreen::ViewController::GotoGameState()
{
  //gonna need the screen controller.
  std::vector<std::string > args;
  m_screenController.GotoScreen( "_OPTIONS_TO_GAME_", args );
}