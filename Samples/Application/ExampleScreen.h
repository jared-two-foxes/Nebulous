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
private:
  RenderSystem* m_renderSystem; ///< The rendering system. 
  Camera*       m_camera;       ///< The camera for scene. 
  Real          m_rotation;
  Material*     m_material;

  public:
    /** \name Structors */ ///@{
    ExampleScreen( const std::string& name, RenderSystem* renderSystem );
    virtual ~ExampleScreen();
    //@}
    
    /** \name Mutators */ ///@{
    virtual void Enter( StateStack* caller );
    virtual void Exit( StateStack* caller );
    virtual void Update( float fDeltaTimeStep, StateStack* pCaller );
    virtual void Render() const;
     //@}

    /** \name Accessors */ ///@{
    const RenderSystem*   GetRenderSystem() const   { return m_renderSystem; }
    const Camera*         GetCamera() const         { return m_camera; }
    //@}

  protected:
    void DrawCube( Matrix4& mvp, Matrix4& normal, Vector4& diffuseColour ) const;

}; //ExampleScreen


} //Nebulae

#endif // __APPLICATION_EXAMPLESCREEN_H__
