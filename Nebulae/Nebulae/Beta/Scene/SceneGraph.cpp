
#include "SceneGraph.h"

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Beta/RenderQueue/RenderQueue.h>
#include <Nebulae/Beta/Scene/ConstantBuffers.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

using namespace Nebulae;
  

class SceneObjectGreater
{
public:
  bool operator() ( const SceneObject* pObj1, const SceneObject* pObj2 ) const
  {
    float a = pObj1->GetNode()->GetPosition().z;
    float b = pObj2->GetNode()->GetPosition().z;
    return ( a < b );
  }
};


//constructor
SceneGraph::SceneGraph( RenderSystemPtr pRenderSystem )
  : m_pRenderQueue( NULL )
  , m_pRenderSystem( pRenderSystem )
  , m_pCameraInProgress( NULL )
  , m_RootSceneNode( NULL )
{}


SceneGraph::~SceneGraph()
{
  Clear();

  if( m_pRenderQueue ) {
    delete m_pRenderQueue;
  }
      
  m_pRenderSystem.reset();
}


RenderQueue*
SceneGraph::GetRenderQueue() const
{ return m_pRenderQueue; }


SceneGraph::RenderSystemPtr
SceneGraph::GetRenderSystem() const
{ return m_pRenderSystem; }


SceneNode* 
SceneGraph::GetRootSceneNode() const
{ return m_RootSceneNode; }


void 
SceneGraph::Clear()
{
  for( std::size_t i = 0, n = m_Nodes.size(); i<n; ++i ) {
    delete m_Nodes[i];
  }
  m_Nodes.clear();

  // Create root SceneNode
  m_RootSceneNode = CreateSceneNode( "root" );
}


bool 
SceneGraph::Initialize()
{
  // Create root SceneNode
  m_RootSceneNode = CreateSceneNode( "root" );

  // Create a RenderQueue
  m_pRenderQueue = new RenderQueue();

  return true;
}


SceneNode* 
SceneGraph::CreateSceneNode()
{
  SceneNode* node = new SceneNode( this );
  //node->SetTransform( Transform::getIdentity() );
  m_Nodes.push_back(node);
  return node;
}


SceneNode* 
SceneGraph::CreateSceneNode( const std::string& name )
{
  SceneNode* node = CreateSceneNode();
  node->SetName( name );
  return node;
}


void 
SceneGraph::RemoveSceneNode( SceneNode* pNode )
{
  // Remove all its child nodes.
  for( std::size_t i = 0, n = pNode->GetChildCount(); i < n; ++i ) {
    SceneNode* pChildNode = pNode->GetChild( i );
    RemoveSceneNode( pChildNode );
  }

  // If it has a parent remove it from its parent.
  SceneNode* pParent = pNode->GetParent();
  if( pParent ) {
    pParent->RemoveChild( pNode );
  }
  pNode->SetParent( NULL );

  // Remove it from node list.
  std::vector<SceneNode*>::iterator end_it = m_Nodes.end();
  for( std::vector<SceneNode*>::iterator it = m_Nodes.begin(); it != end_it; ++it ) {
    if( (*it) == pNode ) {
      m_Nodes.erase( it );
      break;
    }
  }

  delete pNode;
}


void 
SceneGraph::Render( Camera* pCamera )
{
  PROFILE;

  // Clear the render queue.
  PrepareRenderQueue_();
  // Populate the list.
  FindVisibleObjects_( pCamera );
  // Render the list.
  RenderVisibleObjects_( pCamera );
}


void 
SceneGraph::PrepareRenderQueue_()
{ m_pRenderQueue->Clear(); }


void 
SceneGraph::FindVisibleObjects_( Camera* pCamera )
{ m_RootSceneNode->FindVisibleObjects_( pCamera, m_pRenderQueue ); }


void 
SceneGraph::RenderVisibleObjects_( Camera* pCamera )
{
  // Render each separate queue
  RenderQueue::LayersList layers = m_pRenderQueue->GetQueueLayers_();
      
  for( std::size_t i = 0, n = layers.size(); i<n; ++i ) {
    // Skip this one if not to be processed
    RenderQueueLayer* layer = layers[i];
//	if( !layer->IsRenderQueueToBeProcessed(qId) )
//	{
//		continue;
//	}

    RenderQueueGroupObjects_( layer, pCamera );
  }
}


void 
SceneGraph::RenderQueueGroupObjects_( RenderQueueLayer* group, Camera* pCamera )
{	
  // Do Solids
  RenderObjects_( group->GetRenderables(), pCamera );
  // Do transparents (alwas decending) 
  //RenderObjects_(group->getTransparents());
}


void 
SceneGraph::RenderObjects_( std::vector<SceneObject*>& objects, Camera* camera )
{
  if( objects.size() > 0 ) {
    //Sort by z depth.
    std::sort( objects.begin(), objects.end(), SceneObjectGreater() );
      
    for( std::size_t i = 0, n = objects.size(); i<n; ++i ) {
      objects[i]->PreRender( camera );
      objects[i]->Render( m_pRenderSystem );
    }
  }
}
