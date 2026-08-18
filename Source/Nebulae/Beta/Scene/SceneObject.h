#ifndef NEBULAE_BETA_SCENE_SCENEOBJECT_H_
#define NEBULAE_BETA_SCENE_SCENEOBJECT_H_

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/SortKey.h>

namespace Nebulae
{

// Forward Deceleration
class Camera;
class Material;
class Geometry;
class InputLayout;
class RenderSystem;
class SceneNode;

struct RenderSlot
{
  const Material* material = nullptr;
  Geometry* geometry = nullptr;
  InputLayout* inputLayout = nullptr;
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
  bool m_visible;
  std::vector<RenderSlot> m_slots;

public:
  explicit SceneObject( SceneNode* parent );
  ~SceneObject();

  int GetIdentifier() const;
  SceneNode* GetNode() const;
  bool IsVisible() const;
  std::size_t AddSlot( const Material* material );
  std::size_t GetSlotCount() const;
  const RenderSlot& GetSlot( std::size_t index ) const;
  void SetVisible( bool bVisible );
  void AddProvider( const std::string& key, UniformProvider provider );
  void SetSlotGeometry( std::size_t slotIndex, Geometry* geometry );
  void SetSlotInputLayout( std::size_t slotIndex, InputLayout* inputLayout );
  void EmitDrawItems( DrawItemList& items, int layer, int depth );

}; // SceneObject

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_SCENEOBJECT_H_
