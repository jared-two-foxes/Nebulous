#ifndef NEBULAE_SIMPLE_SIMPLESTATE_H_
#define NEBULAE_SIMPLE_SIMPLESTATE_H_

#include <Nebulae/Beta/StateStack/State.h>
#include <Nebulae/Common/Common.h>

namespace Nebulae
{
class Camera;
class RenderSystem;
} // namespace Nebulae

namespace Sample
{
/** GuiSampleState.
 */
class SimpleState : public Nebulae::State
{
public:
  typedef std::shared_ptr<Nebulae::RenderSystem> RenderSystemPtr;
  typedef std::shared_ptr<Nebulae::Camera> CameraPtr;

private:
  RenderSystemPtr m_pRenderSystem; ///< The rendering system.
  CameraPtr m_pCamera;             ///< The camera for scene.


public:
  /** \name Structors */ ///@{
  SimpleState( const std::string& name );
  virtual ~SimpleState();
  //@}

  /** \name Mutators */ ///@{
  virtual void Enter( Nebulae::StateStack* caller );
  virtual void Exit( Nebulae::StateStack* caller );
  virtual void Update( float fDeltaTimeStep, Nebulae::StateStack* pCaller );
  virtual void Render() const;
  //@}

  /** \name Accessors */ ///@{
  CameraPtr GetCamera() const { return m_pCamera; }
  RenderSystemPtr GetRenderSystem() const { return m_pRenderSystem; }
  //@}

}; // GuiSampleState

} // namespace Sample

#endif // NEBULAE_SIMPLE_SIMPLESTATE_H_
