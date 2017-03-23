#ifndef __NEBULAE_SCENEGRAPH_H__
#define __NEBULAE_SCENEGRAPH_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

//Forward deceleration
class Camera;
class RenderQueue;
class RenderQueueLayer;
class RenderSystem;
class SceneNode;
class SceneObject;

/** SceneGraph. 
	*/ 
class SceneGraph
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  std::vector<SceneNode* > m_Nodes;             ///< List of the nodes in the graph.
  SceneNode*               m_RootSceneNode;     ///< The first node, node-0 if you will.
  Camera*                  m_pCameraInProgress; ///< Camera in progress
  RenderQueue*             m_pRenderQueue;      ///< Object used for collecting and ordering render operations.
  RenderSystemPtr          m_pRenderSystem;     ///< Destination RenderSystem
  
	public:
		SceneGraph( RenderSystemPtr pRenderSystem );
		virtual ~SceneGraph();

    //getters
    RenderQueue*    GetRenderQueue() const;
    RenderSystemPtr GetRenderSystem() const;
    SceneNode*      GetRootSceneNode() const;

		//functions
		void Clear();
		bool Initialize();
		void Destroy();
    void Render( Camera* camera );
		SceneNode* CreateSceneNode();
		SceneNode* CreateSceneNode( const std::string& name );
		void RemoveSceneNode( SceneNode* node );
		
	private:
    void PrepareRenderQueue_();
    void FindVisibleObjects_( Camera* pCamera );
    void RenderVisibleObjects_( Camera* pCamera );
    void RenderQueueGroupObjects_( RenderQueueLayer* group, Camera* pCamera );
    void RenderObjects_( std::vector<SceneObject*>& objects, Camera* pCamera );
  
  
}; //SceneGraph

} //Nebulae

#endif // __NEBULAE_SCENEGRAPH_H__