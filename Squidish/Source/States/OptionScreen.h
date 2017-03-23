#ifndef CONTROLLER_STATES_OPTIONSTATE_H__
#define CONTROLLER_STATES_OPTIONSTATE_H__

#include <States/BaseScreen.h>

namespace Game
{

class OptionScreen : public BaseScreen
{
public:
  class ViewController;

private:
  ViewController* m_viewController;
  
  public:
    /** \name Structors */ ///@{
    OptionScreen( ScreenController* owner, const FileArchivePtr& fileSystem, Dispatcher& dispatcher,
      const AudioControllerPointer& audioController, const GuiControllerPointer& guiMgr,
      const ModelPointer& model, const ViewPointer& view, ViewController* viewController );
    
    virtual ~OptionScreen();
    //@}
  
  private:
    virtual void OnEnter();
    virtual void OnExit();

};

}

#endif // CONTROLLER_STATES_OPTIONSTATE_H__
