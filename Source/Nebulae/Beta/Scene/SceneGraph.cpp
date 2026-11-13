
#include "SceneGraph.h"

#include <algorithm>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/Scene/ConstantBuffers.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/RenderQueue/StreamCompiler.h>

using namespace Nebulae;


// constructor
SceneGraph::SceneGraph( RenderSystemPtr pRenderSystem )
  : m_pRenderSystem( pRenderSystem ),
    m_pCameraInProgress( nullptr ),
    m_RootSceneNode( nullptr ),
    m_nextDrawItemSourceId( 0 )
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
  if ( !m_RootSceneNode || !m_pRenderSystem )
  {
    return;
  }
  DrawItemList items;
  m_RootSceneNode->TraverseNode( items );
  for ( const auto& source : m_drawItemSources )
  {
    source.second( items );
  }
  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, pCamera, stream );
  m_pRenderSystem->ExecuteStream( stream );
}


std::size_t SceneGraph::AddDrawItemSource( DrawItemSource source )
{
  const std::size_t id = m_nextDrawItemSourceId++;
  m_drawItemSources.emplace_back( id, std::move( source ) );
  return id;
}


void SceneGraph::RemoveDrawItemSource( std::size_t id )
{
  m_drawItemSources.erase(
    std::remove_if( m_drawItemSources.begin(), m_drawItemSources.end(),
                    [id]( const std::pair<std::size_t, DrawItemSource>& s ) { return s.first == id; } ),
    m_drawItemSources.end() );
}
