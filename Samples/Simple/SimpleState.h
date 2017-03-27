#ifndef __NEBULAE_GUISAMPLESTATE_H__
#define __NEBULAE_GUISAMPLESTATE_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/State.h>

namespace Nebulae {
  class Camera;
  class DebugUtil;
  class GuiManager;
  class RenderSystem;
}

namespace Sample
{ 
  /** GuiSampleState.
   */
  class SimpleState : public Nebulae::State
  {
  private:
    Nebulae::RenderSystem* m_pRenderSystem; ///< The rendering system.
    Nebulae::Camera*       m_pCamera;       ///< The camera for scene.
    Nebulae::DebugUtil*    m_debugRenderer; ///< The renderer used for drawing simple debug shapes.

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
      Nebulae::Camera*         GetCamera() const         { return m_pCamera; }
      Nebulae::RenderSystem*   GetRenderSystem() const   { return m_pRenderSystem; }
      //@}

  }; //GuiSampleState

} //Nebulae

#endif // __NEBULAE_GUISAMPLESTATE_H__
