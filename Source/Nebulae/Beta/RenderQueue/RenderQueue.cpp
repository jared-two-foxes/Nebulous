#include "RenderQueue.h"

namespace Nebulae
{
	//--------------------------------------------------------------------------------------
	RenderQueue::RenderQueue()
		: m_DefaultLayer(0)
	{
		m_Layers.push_back(new RenderQueueLayer(this));
	}
	//--------------------------------------------------------------------------------------
	RenderQueue::~RenderQueue()
	{
		for ( std::size_t i = 0, n = m_Layers.size(); i<n; i++ )
		{
			delete m_Layers[i];
		}
    m_Layers.clear();
	}
	//--------------------------------------------------------------------------------------
	void RenderQueue::Clear()
	{
		for ( std::size_t i = 0, n = m_Layers.size(); i<n; ++i )
		{
			m_Layers[i]->Clear();
		}
	}
	//--------------------------------------------------------------------------------------
	void RenderQueue::AddRenderable( SceneObject* r, int layer )
	{
		//TODO: handle the priority variable.
		m_Layers[layer]->AddRenderable(r);
	}
	//--------------------------------------------------------------------------------------
	void RenderQueue::AddRenderable( SceneObject* r )
	{
		AddRenderable( r, m_DefaultLayer );
	}
} //Nebulae