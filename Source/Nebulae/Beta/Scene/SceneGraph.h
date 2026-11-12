#ifndef NEBULAE_BETA_SCENE_SCENEGRAPH_H_
#define NEBULAE_BETA_SCENE_SCENEGRAPH_H_

#include <Nebulae/Common/Common.h>

#include <functional>
#include <utility>
#include <vector>

namespace Nebulae
{

// Forward deceleration
class Camera;
class DrawItemList;
class RenderSystem;
class SceneNode;
class SceneObject;

/** SceneGraph.
 */
class SceneGraph
{
public:
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

  /** Callback which appends additional DrawItems (e.g. particles) to the frame's collection. */
  typedef std::function<void( DrawItemList& )> DrawItemSource;

private:
  std::vector<SceneNode*> m_Nodes; ///< List of the nodes in the graph.
  SceneNode* m_RootSceneNode;      ///< The first node, node-0 if you will.
  Camera* m_pCameraInProgress;     ///< Camera in progress
  RenderSystemPtr m_pRenderSystem; ///< Destination RenderSystem
  std::vector<std::pair<std::size_t, DrawItemSource>> m_drawItemSources; ///< Extra per-frame DrawItem producers.
  std::size_t m_nextDrawItemSourceId; ///< Identifier handed out to the next registered source.

public:
  SceneGraph( RenderSystemPtr pRenderSystem );
  virtual ~SceneGraph();

  // getters
  RenderSystemPtr GetRenderSystem() const;
  SceneNode* GetRootSceneNode() const;

  // functions
  void Clear();
  bool Initialize();
  void Destroy();
  void Render( Camera* camera );

  /** Registers a producer whose DrawItems are added to the same list as the scene's own before compilation.
   *  Returns an id for RemoveDrawItemSource; remove the source before whatever it captures is destroyed. */
  std::size_t AddDrawItemSource( DrawItemSource source );
  void RemoveDrawItemSource( std::size_t id );
  SceneNode* CreateSceneNode();
  SceneNode* CreateSceneNode( const std::string& name );
  void RemoveSceneNode( SceneNode* node );

private:

}; // SceneGraph

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_SCENEGRAPH_H_
