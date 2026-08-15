#ifndef NEBULAE_BETA_SCENE_SCENEOBJECT_H_
#define NEBULAE_BETA_SCENE_SCENEOBJECT_H_

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Scene/UniformParameters.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>

namespace Nebulae
{

// Forward Deceleration
class Camera;
class Material;
class Geometry;
class InputLayout;
class RenderSystem;
class SceneNode;

struct PassData
{
  Geometry* Geometry;
  InputLayout* VertexLayout;
  int RenderTargetViewCount;
};

struct RenderSlot
{
  const Material* material;
  std::vector<std::pair<std::string, UniformProvider>> providers;
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
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

private:
  static int ms_nextIdentifier;

  int m_identifier;
  SceneNode* m_node;
  const Material* m_material;
  std::vector<PassData*> m_passData;
  UniformParameters m_uniforms;
  bool m_visible;
  std::vector<RenderSlot> m_slots;

public:
  explicit SceneObject( SceneNode* parent, const Material* pMaterial );
  explicit SceneObject( SceneNode* parent );
  ~SceneObject();

  // getters
  int GetIdentifier() const;
  SceneNode* GetNode() const;
  bool IsVisible() const;
  const Material* GetMaterial() const;
  UniformParameters& GetUniformParameters();

  // functions
  void Clear();
  std::size_t AddSlot( const Material* material );
  std::size_t GetSlotCount() const;
  const RenderSlot& GetSlot( std::size_t index ) const;
  bool Initialize();
  void SetVisible( bool bVisible );
  void SetGeometry( std::size_t iPass, Geometry* pGeometry );
  void SetInputLayout( std::size_t iPass, InputLayout* pInputLayout );
  void PreRender( Camera* pCamera );
  void Render( RenderSystemPtr renderSystem ) const;
  void AddProvider( const std::string& key, UniformProvider provider );

}; // SceneObject

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_SCENEOBJECT_H_
