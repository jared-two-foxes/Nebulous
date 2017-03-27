#ifndef RENDERQUEUE_H__
#define RENDERQUEUE_H__

#include <Nebulae/Common/Base/Base.h>
#include "RenderQueueLayer.h"

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

	}; //RenderQueue

} // Nebulae

#endif // RENDERQUEUE_H__