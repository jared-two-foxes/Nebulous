#ifndef CONTROLLER_STATES_PAUSESTATE_H__
#define CONTROLLER_STATES_PAUSESTATE_H__

#include <States/BaseScreen.h>

namespace Game {

class PauseScreen : public BaseScreen
{
  public:
    /** \name Structors */ ///@{
    PauseScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
      const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
      const ModelPointer& model, const ViewPointer& view );
    
    virtual ~PauseScreen();
    //@}
    
};

}

#endif // CONTROLLER_STATES_PAUSESTATE_H__
