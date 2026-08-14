
#include "SceneGraph.h"

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Beta/Scene/ConstantBuffers.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

using namespace Nebulae;


// constructor
SceneGraph::SceneGraph( RenderSystemPtr pRenderSystem )
  : m_pRenderSystem( pRenderSystem ), m_pCameraInProgress( nullptr ), m_RootSceneNode( nullptr )
{
}


SceneGraph::~SceneGraph()
{
  Clear();

  m_pRenderSystem.reset();
}


SceneGraph::RenderSystemPtr SceneGraph::GetRenderSystem() const { return m_pRenderSystem; }


SceneNode* SceneGraph::GetRootSceneNode() const { return m_RootSceneNode; }


void SceneGraph::Clear()
{
  for ( auto& node : m_Nodes )
  {
    delete node;
  }
  m_Nodes.clear();

  // Create root SceneNode
  m_RootSceneNode = CreateSceneNode( "root" );
}


bool SceneGraph::Initialize()
{
  // Create root SceneNode
  m_RootSceneNode = CreateSceneNode( "root" );

  return true;
}


SceneNode* SceneGraph::CreateSceneNode()
{
  SceneNode* node = new SceneNode( this );
  // node->SetTransform( Transform::getIdentity() );
  m_Nodes.push_back( node );
  return node;
}


SceneNode* SceneGraph::CreateSceneNode( const std::string& name )
{
  SceneNode* node = CreateSceneNode();
  node->SetName( name );
  return node;
}


void SceneGraph::RemoveSceneNode( SceneNode* pNode )
{
  // Remove all its child nodes.
  for ( std::size_t i = 0, n = pNode->GetChildCount(); i < n; ++i )
  {
    SceneNode* pChildNode = pNode->GetChild( i );
    RemoveSceneNode( pChildNode );
  }

  // If it has a parent remove it from its parent.
  SceneNode* pParent = pNode->GetParent();
  if ( pParent )
  {
    pParent->RemoveChild( pNode );
  }
  pNode->SetParent( nullptr );

  // Remove it from node list.
  std::vector<SceneNode*>::iterator end_it = m_Nodes.end();
  for ( std::vector<SceneNode*>::iterator it = m_Nodes.begin(); it != end_it; ++it )
  {
    if ( ( *it ) == pNode )
    {
      m_Nodes.erase( it );
      break;
    }
  }

  delete pNode;
}


void SceneGraph::Render( Camera* pCamera )
{
  // Clear the render queue.
  PrepareRenderQueue_();
  // Populate the list.
  FindVisibleObjects_( pCamera );
  // Render the list.
  RenderVisibleObjects_( pCamera );
}


void SceneGraph::PrepareRenderQueue_() {}


void SceneGraph::FindVisibleObjects_( Camera* pCamera )
{
  // TODO(Phase 5.1, SA-441): emit a DrawItem instead of queueing SceneObject.
}


void SceneGraph::RenderVisibleObjects_( Camera* pCamera )
{
  // TODO(Phase 5.1, SA-441): compile DrawItems into a RenderStream
}
