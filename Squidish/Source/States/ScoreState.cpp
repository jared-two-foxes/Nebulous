
#include "ScoreState.h"

using namespace Game;


ScoreScreen::ScoreScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
  const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
  const ModelPointer& model, const ViewPointer& view ) 
///
/// Constructor.
///
  : BaseScreen( "ScoreScreen", owner, fileSystem, dispatcher, audioController, guiMgr, model, view )
{
}


ScoreScreen::~ScoreScreen()
///
/// Destructor
///
{
}