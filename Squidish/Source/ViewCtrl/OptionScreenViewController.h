#ifndef __CONTROLLER_VIEWCONTROLLERS_OPTIONSCREENVIEWCONTROLLER_H__ 
#define __CONTROLLER_VIEWCONTROLLERS_OPTIONSCREENVIEWCONTROLLER_H__ 

#include <States/OptionScreen.h>
#include <boost/signals2.hpp>

namespace Nebulae 
{
  class SpriteAtlasManager;
}

namespace Game 
{

class OptionScreen::ViewController
{
public:
  typedef std::shared_ptr<Nebulae::SpriteAtlasManager > SpriteAtlasPtr;

private:
  ScreenController&                  m_screenController;
  BaseScreen::AudioControllerPointer m_audioController;

public:
  boost::signals2::scoped_connection m_backButtonConnection;

  public:
    ViewController( ScreenController& screenController );

    void SetAudioController( AudioControllerPointer audioController );

    virtual void Setup( BaseScreen::RenderSystemPointer renderSystem, SpriteAtlasPtr atlasManagerPtr, BaseScreen::GuiControllerPointer gui );
  
    void MusicVolumeSliderMoved( Real current, Real min, Real max );
     
    void GotoGameState();

};

} 

#endif // __CONTROLLER_VIEWCONTROLLERS_OPTIONSCREENVIEWCONTROLLER_H__ 