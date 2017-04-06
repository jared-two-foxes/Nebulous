#ifndef __APPLICATION_EXAMPLESCREEN_H__
#define __APPLICATION_EXAMPLESCREEN_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/StateStack/State.h>

namespace Nebulae
{

class Camera;
class RenderSystem;
class Material;


class ExampleScreen : public State
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
  typedef std::shared_ptr<Camera >       CameraPtr;

private:
  RenderSystemPtr m_renderSystem; ///< The rendering system. 
  CameraPtr       m_camera;       ///< The camera for scene. 
  Real            m_rotation;

  public:
    /** \name Structors */ ///@{
    ExampleScreen( const std::string& name, RenderSystemPtr renderSystem );
    virtual ~ExampleScreen();
    //@}
    
    /** \name Mutators */ ///@{
    virtual void Enter( StateStack* caller );
    virtual void Exit( StateStack* caller );
    virtual void Update( float fDeltaTimeStep, StateStack* pCaller );
    virtual void Render() const;
     //@}

    /** \name Accessors */ ///@{
    const RenderSystemPtr   GetRenderSystem() const   { return m_renderSystem; }
    const CameraPtr         GetCamera() const         { return m_camera; }
    //@}

  protected:
    void DrawCube( Matrix4& mvp, Matrix4& normal, Vector4& diffuseColour ) const;

}; //ExampleScreen


} //Nebulae

#endif // __APPLICATION_EXAMPLESCREEN_H__
