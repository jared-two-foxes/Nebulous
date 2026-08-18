
#include "SceneNode.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/RenderQueue/RenderQueue.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/Scene/SceneObject.h>

using namespace Nebulae;


// constructor
SceneNode::SceneNode( SceneGraph* creator, const std::string& name )
  : m_strName( name ),
    m_pCreator( creator ),
    m_pParent( nullptr ),
    m_Position( 0.0f, 0.0f, 0.0f ),
    m_Rotation( 0.0f, 0.0f, 0.0f, 1.0f ),
    m_Scale( 1.0f, 1.0f, 1.0f ),
    m_bVisible( true )
{
}

// destructor
SceneNode::~SceneNode() { Clear(); }


bool SceneNode::IsVisible() const { return m_bVisible; }


const Vector4& SceneNode::GetPosition() const { return m_Position; }


const Quaternion& SceneNode::GetRotation() const { return m_Rotation; }


const Vector4& SceneNode::GetScale() const { return m_Scale; }


SceneNode* SceneNode::GetParent() const { return m_pParent; }


std::size_t SceneNode::GetChildCount() const { return m_ChildNodes.size(); }


SceneNode* SceneNode::GetChild( size_t idx )
{
  NE_ASSERT( idx < m_ChildNodes.size(), "Requested index is out of bounds." );
  return m_ChildNodes[idx];
}


const SceneNode* SceneNode::GetChild( size_t idx ) const
{
  NE_ASSERT( idx < m_ChildNodes.size(), "Requested index is out of bounds." );
  return m_ChildNodes[idx];
}


void SceneNode::GetWorldMatrix( Matrix4* pWorldMatrixOut ) const
{
  /// Get the matrix from the parent if there is one.
  if ( m_pParent != nullptr )
  {
    m_pParent->GetWorldMatrix( pWorldMatrixOut );
  }

  /// Calculate the local transform
  Matrix4 trans = MatrixMakeTranslation( m_Position.x, m_Position.y, m_Position.z );
  Matrix4 scale = MatrixMakeScale( m_Scale.x, m_Scale.y, m_Scale.z );

  Matrix4 local;
  local.SetIdentity();
  local = ( scale * trans );

  /// Multiply with parent to get full matrix.
  ( *pWorldMatrixOut ) = local * ( *pWorldMatrixOut );
}


void SceneNode::Clear()
{
  std::vector<SceneObject*>::iterator end_it = m_Objects.end();
  for ( std::vector<SceneObject*>::iterator it = m_Objects.begin(); it != end_it; ++it )
  {
    delete ( *it );
  }
  m_Objects.clear();
}


void SceneNode::SetName( const std::string& name ) { m_strName = name; }


void SceneNode::SetVisible( bool visible ) { m_bVisible = visible; }


void SceneNode::SetPosition( const Vector4& position ) { m_Position = position; }


void SceneNode::SetRotation( const Quaternion& rotation ) { m_Rotation = rotation; }


void SceneNode::SetScale( const Vector4& scale ) { m_Scale = scale; }


void SceneNode::SetParent( SceneNode* pParent ) { m_pParent = pParent; }


SceneNode* SceneNode::CreateChild( const std::string& name )
{
  SceneNode* node = CreateChild();
  // TODO: Add name to node.
  return node;
}


SceneNode* SceneNode::CreateChild()
{
  SceneNode* node = m_pCreator->CreateSceneNode();
  m_ChildNodes.push_back( node );
  node->SetParent( this );
  return node;
}


void SceneNode::RemoveChild( SceneNode* pNode )
{
  std::vector<SceneNode*>::iterator end_it = m_ChildNodes.end();
  for ( std::vector<SceneNode*>::iterator it = m_ChildNodes.begin(); it != end_it; ++it )
  {
    if ( ( *it ) == pNode )
    {
      m_ChildNodes.erase( it );
      break;
    }
  }
}


SceneObject* SceneNode::CreateObject( const Material* pMaterial )
{
  SceneObject* pObj = new SceneObject( this );
  pObj->AddSlot( pMaterial );
  m_Objects.push_back( pObj );
  return pObj;
}


bool SceneNode::DetachObject( SceneObject* pObject )
{
  std::vector<SceneObject*>::iterator end_it = m_Objects.end();
  for ( std::vector<SceneObject*>::iterator it = m_Objects.begin(); it != end_it; ++it )
  {
    if ( ( *it ) == pObject )
    {
      //@todo [jared.watt 18/02/2013] We will need to flag it as dead and then garbage collect it I think.

      // m_Objects.erase( it );
      // delete pObject;

      return true;
    }
  }

  return false;
}


SceneObject* SceneNode::FindSubObject( const Material* material ) const
{
  for ( auto object : m_Objects )
  {
    for ( std::size_t slotIndex = 0, slotCount = object->GetSlotCount(); slotIndex < slotCount; ++slotIndex )
    {
      if ( object->GetSlot( slotIndex ).material == material )
      {
        return object;
      }
    }
  }
  return nullptr;
}


void SceneNode::FindVisibleObjects_( Camera* camera, RenderQueue* renderQueue )
{
  // @todo: check if node is in the frustum (i.e. Do Culling)

  if ( m_bVisible )
  {
    for ( auto& object : m_Objects )
    {
      if ( object->IsVisible() )
      {
        // TODO(Phase 5.1, SA-441): emit a DrawItem instead of quenueing the
        // object directly. The DrawItem will contain a pointer to the
        // SceneObject and a sort key.
      }
    }

    for ( auto& childNode : m_ChildNodes )
    {
      childNode->FindVisibleObjects_( camera, renderQueue );
    }
  }
}
