#ifndef NEBULAE_BETA_RENDERQUEUE_RENDERQUEUELAYER_H_
#define NEBULAE_BETA_RENDERQUEUE_RENDERQUEUELAYER_H_

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
// Forward Decleration
class Camera;
class SceneObject;
class RenderQueue;

class RenderQueueLayer
{
public:
  typedef std::vector<SceneObject*> RenderableList;

protected:
  RenderQueue* m_Parent;
  RenderableList m_SolidsBasic;

public:
  explicit RenderQueueLayer( RenderQueue* parent );
  ~RenderQueueLayer();

  void Clear();

  RenderableList& GetRenderables() { return m_SolidsBasic; }

  void AddRenderable( SceneObject* r );

  void Sort( Camera* cam );

}; // RenderQueueLayer

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_RENDERQUEUELAYER_H_