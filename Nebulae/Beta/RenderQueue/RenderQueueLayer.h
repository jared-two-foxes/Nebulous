#ifndef RENDERQUEUELAYER_H__
#define RENDERQUEUELAYER_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
	//Forward Decleration
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

	}; //RenderQueueLayer

} //Nebulae

#endif // RENDERQUEUELAYER_H__