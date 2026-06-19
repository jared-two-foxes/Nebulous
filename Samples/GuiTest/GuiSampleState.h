#ifndef NEBULAE_GUITEST_GUISAMPLESTATE_H_
#define NEBULAE_GUITEST_GUISAMPLESTATE_H_

#include <Nebulae/Beta/StateStack/State.h>
#include <Nebulae/Common/Common.h>

namespace Nebulae
{
class Camera;
class FontManager;
class GuiInputListener;
class GuiManager;
class RenderSystem;
class WidgetRenderer;

/** GuiSampleState.
 */
class GuiSampleState : public State
{
public:
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

private:
  RenderSystemPtr m_pRenderSystem; /// The rendering system. Strong Reference.
  Camera* m_pCamera;               /// The camera for scene. Strong Reference.
  GuiManager* m_pGuiManager;       /// The gui manager. Strong Reference.
  GuiInputListener* m_inputListener;
  FontManager* m_fontMgr;
  WidgetRenderer* m_renderer;

public:
  /** \name Structors */ ///@{
  GuiSampleState( const std::string& name );
  virtual ~GuiSampleState();
  //@}

  /** \name Mutators */ ///@{
  virtual void Enter( StateStack* caller );
  virtual void Exit( StateStack* caller );
  virtual void Update( float fDeltaTimeStep, StateStack* pCaller );
  virtual void Render() const;
  //@}

  /** \name Accessors */ ///@{
  Camera* GetCamera() const { return m_pCamera; }
  const RenderSystemPtr GetRenderSystem() const { return m_pRenderSystem; }
  //@}

private:
  void SetupInternalSystems( StateStack* caller );
  void SetupGuiComponents( StateStack* caller );

}; // GuiSampleState

} // namespace Nebulae

#endif // NEBULAE_GUITEST_GUISAMPLESTATE_H_
