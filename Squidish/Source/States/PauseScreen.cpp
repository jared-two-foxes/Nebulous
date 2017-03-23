
#include "PauseScreen.h"

using namespace Game;


PauseScreen::PauseScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
  const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
  const ModelPointer& model, const ViewPointer& view ) 
///
/// Constructor.
///
  : BaseScreen( "PauseScreen", owner, fileSystem, dispatcher, audioController, guiMgr, model, view )
{
}


PauseScreen::~PauseScreen()
///
/// Destructor
///
{
}