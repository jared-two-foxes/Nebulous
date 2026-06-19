#ifndef NEBULAE_BETA_RENDERQUEUE_RENDERQUEUE_H_
#define NEBULAE_BETA_RENDERQUEUE_RENDERQUEUE_H_

#include "RenderQueueLayer.h"
#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{

class RenderQueue
{
public:
  typedef std::vector<RenderQueueLayer*> LayersList;

protected:
  LayersList m_Layers;
  unsigned int m_DefaultLayer;

public:
  RenderQueue();
  ~RenderQueue();

  void Clear();

  void AddRenderable( SceneObject* r, int layer );
  void AddRenderable( SceneObject* r );

  LayersList& GetQueueLayers_() { return m_Layers; }

}; // RenderQueue

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_RENDERQUEUE_H_