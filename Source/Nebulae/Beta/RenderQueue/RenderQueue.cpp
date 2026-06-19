#include "RenderQueue.h"

namespace Nebulae
{
//--------------------------------------------------------------------------------------
RenderQueue::RenderQueue() : m_DefaultLayer( 0 ) { m_Layers.push_back( new RenderQueueLayer( this ) ); }
//--------------------------------------------------------------------------------------
RenderQueue::~RenderQueue()
{
  for ( auto& layer : m_Layers )
  {
    delete layer;
  }
  m_Layers.clear();
}
//--------------------------------------------------------------------------------------
void RenderQueue::Clear()
{
  for ( auto& layer : m_Layers )
  {
    layer->Clear();
  }
}
//--------------------------------------------------------------------------------------
void RenderQueue::AddRenderable( SceneObject* r, int layer )
{
  // TODO: handle the priority variable.
  m_Layers[layer]->AddRenderable( r );
}
//--------------------------------------------------------------------------------------
void RenderQueue::AddRenderable( SceneObject* r ) { AddRenderable( r, m_DefaultLayer ); }
} // namespace Nebulae
