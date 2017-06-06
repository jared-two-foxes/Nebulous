#ifndef SCENEOBJECT_H__
#define SCENEOBJECT_H__

#include <Nebulae/Common/Common.h>
#include <NEbulae/Beta/Scene/UniformParameters.h>

namespace Nebulae {

// Forward Deceleration
class Camera;
class Material;
class Geometry;
class InputLayout;
class RenderSystem;
class SceneNode;
  
struct PassData
{
  Geometry*    Geometry;
  InputLayout* VertexLayout;
  int          RenderTargetViewCount;
};

/** 
  *  An object that represents a single render operation at the specified scene node location.
  *
  *  @issues 
  *    Reintroduce the concept of subobjects here such that we can have multiple objects 
  *    using the same material attached to a single node.
	*/
class SceneObject
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  static int ms_nextIdentifier;
  
  int                     m_identifier;
  SceneNode*              m_node;
  const Material*         m_material;
  std::vector<PassData* > m_passData;
  UniformParameters       m_uniforms;
  bool                    m_visible;

  public:
    explicit SceneObject( SceneNode* parent, const Material* pMaterial );
    ~SceneObject();

    //getters
    int        GetIdentifier() const;
    SceneNode* GetNode() const;
    bool       IsVisible() const;
    const Material* GetMaterial() const;
    UniformParameters&  GetUniformParameters();

    // functions
    void Clear();
    bool Initialize();
    void SetVisible( bool bVisible );
    void SetGeometry( std::size_t iPass, Geometry* pGeometry );
    void SetInputLayout( std::size_t iPass, InputLayout* pInputLayout );
    void PreRender( Camera* pCamera );
    void Render( RenderSystemPtr renderSystem ) const;

}; //SceneObject

} //Nebulae

#endif // SCENEOBJECT_H__