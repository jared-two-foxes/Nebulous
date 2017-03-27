#include "RenderQueueLayer.h"

namespace Nebulae
{
	//--------------------------------------------------------------------------------------
	RenderQueueLayer::RenderQueueLayer(RenderQueue* parent)
		: m_Parent(parent)
	{

	}
	//--------------------------------------------------------------------------------------
	RenderQueueLayer::~RenderQueueLayer()
	{
		m_SolidsBasic.clear();
//		m_Transparents.clear();
	}
	//--------------------------------------------------------------------------------------
	void RenderQueueLayer::AddRenderable( SceneObject* r )
	{
		m_SolidsBasic.push_back( r );
	}
	//--------------------------------------------------------------------------------------
	void RenderQueueLayer::Sort( Camera* cam )
	{
		//TODO: implement
	}
	//--------------------------------------------------------------------------------------
	void RenderQueueLayer::Clear()
	{
		m_SolidsBasic.clear();
//		m_Transparents.clear();
	}
} //Nebulae