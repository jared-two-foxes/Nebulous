
#include "OptionScreen.h"

#include "Model/Model.h"
#include "Model/Systems/AudioController.h"
#include "View/View.h"
#include "ViewCtrl/OptionScreenViewController.h"

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Control/Slider.h>

using namespace Game;


OptionScreen::OptionScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
  const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
  const ModelPointer& model, const ViewPointer& view, ViewController* viewController ) 
: BaseScreen( "OptionScreen", owner, fileSystem, dispatcher, audioController, guiMgr, model, view ),
  m_viewController( viewController )
{
  if( m_viewController != nullptr )
  {
    m_viewController->SetAudioController( audioController );
  }
}


OptionScreen::~OptionScreen()
///
/// Destructor
///
{}


void
OptionScreen::OnEnter()
{
  View::RenderSystemPtr       renderDevice = GetView()->GetRenderDevice();
  View::SpriteAtlasManagerPtr atlasManager = GetView()->GetSpriteAtlasManager();

  m_viewController->Setup( renderDevice, atlasManager, GetGuiController() );
}

void
OptionScreen::OnExit()
{

}