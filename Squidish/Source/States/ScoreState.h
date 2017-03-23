#ifndef CONTROLLER_STATES_SCORESTATE_H__
#define CONTROLLER_STATES_SCORESTATE_H__

#include <States/BaseScreen.h>

namespace Game
{

class ScoreScreen : public BaseScreen
{
  public:
    /** \name Structors */ ///@{
    ScoreScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
      const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
      const ModelPointer& model, const ViewPointer& view );

    virtual ~ScoreScreen();
    //@}
    
};

}

#endif // CONTROLLER_STATES_OPTIONSTATE_H__
