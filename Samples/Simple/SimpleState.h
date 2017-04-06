#ifndef __NEBULAE_GUISAMPLESTATE_H__
#define __NEBULAE_GUISAMPLESTATE_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/State.h>

namespace Nebulae {
  class Camera;
  class RenderSystem;
}

namespace Sample
{ 
  /** GuiSampleState.
   */
  class SimpleState : public Nebulae::State
  {
  public:
    typedef std::shared_ptr<Nebulae::RenderSystem > RenderSystemPtr;
    typedef std::shared_ptr<Nebulae::Camera >       CameraPtr;

  private:
    RenderSystemPtr m_pRenderSystem; ///< The rendering system.
    CameraPtr       m_pCamera;       ///< The camera for scene.


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
      CameraPtr         GetCamera() const         { return m_pCamera; }
      RenderSystemPtr   GetRenderSystem() const   { return m_pRenderSystem; }
      //@}

  }; //GuiSampleState

} //Nebulae

#endif // __NEBULAE_GUISAMPLESTATE_H__
