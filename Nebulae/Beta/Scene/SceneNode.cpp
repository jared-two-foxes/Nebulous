
#include "SceneNode.h"

//#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/RenderQueue/RenderQueue.h>

using namespace Nebulae;


//constructor
SceneNode::SceneNode( SceneGraph* creator, const std::string& name )
  : m_strName( name ),
    m_pCreator( creator ),
    m_pParent( NULL ),
    m_Position( 0.0f,0.0f,0.0f ),
    m_Rotation( 0.0f,0.0f,0.0f,1.0f ),
    m_Scale( 1.0f,1.0f,1.0f ),
    m_bVisible( true )
{
	
}

//destructor
SceneNode::~SceneNode()
{
  Clear();
}


bool             
SceneNode::IsVisible() const 
{ return m_bVisible; }


const Vector4& 
SceneNode::GetPosition() const 
{ return m_Position; }

    
const Quaternion& 
SceneNode::GetRotation() const 
{ return m_Rotation; }


const Vector4& 
SceneNode::GetScale() const 
{ return m_Scale; }


SceneNode*       
SceneNode::GetParent() const 
{ return m_pParent; }


std::size_t
SceneNode::GetChildCount() const
{ return m_ChildNodes.size(); }


SceneNode* 
SceneNode::GetChild( size_t idx )
{
  NE_ASSERT( idx < m_ChildNodes.size(), "Requested index is out of bounds." )( idx, m_ChildNodes.size() );
  return m_ChildNodes[idx];
}


const SceneNode* 
SceneNode::GetChild( size_t idx ) const
{
  NE_ASSERT( idx < m_ChildNodes.size(), "Requested index is out of bounds." )( idx, m_ChildNodes.size() );
  return m_ChildNodes[idx];
}


void 
SceneNode::GetWorldMatrix( Matrix4* pWorldMatrixOut ) const
{
  /// Get the matrix from the parent if there is one.
  if( m_pParent != NULL ) {
    m_pParent->GetWorldMatrix( pWorldMatrixOut );
  }

  /// Calculate the local transform
  Matrix4 trans = MatrixMakeTranslation( m_Position.x, m_Position.y, m_Position.z );
  Matrix4 scale = MatrixMakeScale( m_Scale.x, m_Scale.y, m_Scale.z );

  Matrix4 local;
  local.SetIdentity();
  local = ( scale * trans );

  /// Multiply with parent to get full matrix.
  (*pWorldMatrixOut) = local * (*pWorldMatrixOut);
}


void 
SceneNode::Clear()
{
  std::vector<SceneObject*>::iterator end_it = m_Objects.end();
  for( std::vector<SceneObject*>::iterator it = m_Objects.begin(); it != end_it; ++it ) {
	  delete (*it);
  }
  m_Objects.clear();
}


void
SceneNode::SetName( const std::string& name )
{ m_strName = name; }


void 
SceneNode::SetVisible( bool visible ) 
{ m_bVisible = visible; }


void
SceneNode::SetPosition( const Vector4& position )
{ m_Position = position; }


void
SceneNode::SetRotation( const Quaternion& rotation )
{ m_Rotation = rotation; }


void 
SceneNode::SetScale( const Vector4& scale )
{ m_Scale = scale; }


void 
SceneNode::SetParent( SceneNode* pParent ) 
{ m_pParent = pParent; }


SceneNode* 
SceneNode::CreateChild( const std::string& name ) 
{
  SceneNode* node = CreateChild();
  //TODO: Add name to node.
  return node;
}


SceneNode* 
SceneNode::CreateChild() 
{
  SceneNode* node = m_pCreator->CreateSceneNode();
  m_ChildNodes.push_back(node);
  node->SetParent(this);
  return node;
}


void 
SceneNode::RemoveChild( SceneNode* pNode )
{
  std::vector<SceneNode*>::iterator end_it = m_ChildNodes.end();
  for( std::vector<SceneNode*>::iterator it = m_ChildNodes.begin(); it != end_it; ++it ) {
    if( (*it) == pNode ) {
      m_ChildNodes.erase( it );
      break;
    }
  }
}


SceneObject* 
SceneNode::CreateObject( const Material* pMaterial )
{
  SceneObject* pObj = new SceneObject( this, pMaterial );
  pObj->Initialize();
  m_Objects.push_back( pObj );
  return pObj;
}


bool 
SceneNode::DetachObject( SceneObject* pObject )
{
  std::vector<SceneObject*>::iterator end_it = m_Objects.end();
  for( std::vector<SceneObject*>::iterator it = m_Objects.begin(); it != end_it; ++it ) {
    if( (*it) == pObject ) {
      //@todo [jared.watt 18/02/2013] We will need to flag it as dead and then garbage collect it I think.
    
      //m_Objects.erase( it );
      //delete pObject;

      return true;
    }
  }
  
  return false;
}


SceneObject* 
SceneNode::FindSubObject( const Material* material ) const
{
  for( std::size_t i = 0, n = m_Objects.size(); i < n; ++i ) {
    if( m_Objects[i]->GetMaterial() == material ) {
      return m_Objects[i];
    }
  }
  return NULL;
}


void 
SceneNode::FindVisibleObjects_( Camera* camera, RenderQueue* renderQueue )
{
  // @todo: check if node is in the frustum (i.e. Do Culling)
 
  if( m_bVisible ) {
	  for( std::size_t i = 0, n = m_Objects.size(); i<n; i++ ) {
		  if( m_Objects[i]->IsVisible() ) {
			  renderQueue->AddRenderable( m_Objects[i] );
		  }
	  }

	  for( std::size_t i = 0, n = m_ChildNodes.size(); i<n; ++i ) {
		  m_ChildNodes[i]->FindVisibleObjects_( camera, renderQueue );
	  }
  }
}


//AttachMesh
/*SceneObject* SceneNode::AttachMesh( const Mesh* mesh )
{
  //TODO: Maybe make the assumption that the mesh is loaded so we can pass a const reference here?
  assert( mesh->IsLoaded() );

  //TODO: Somehow handle LOD.
  int lod = 0;
  for (int i = 0, n = mesh->GetSubMeshCount(lod); i<n; ++i)
  {
	  SubMesh* sm = mesh->GetSubMesh( lod, i );
	
	  Material* mat = MaterialManager::getSingletonPtr()->GetByName( sm->m_strMaterialName );
	  assert(mat != NULL);

	  if (!mat->IsLoaded())
	  {
		  mat->Load();
	  }

	  ScenebObject* pSubObj = new SceneObject( this, mat );

	  pSubObj->Initialize();

	  //pObj->m_pSubMesh = sm;
	  //pObj->initialize();
		
	  // Convert sm into geometry and attach to objects
	  Geometry* pGeometry = new Geometry();			
	  pGeometry->m_pVertexBuffer	= sm->m_pVertexBuffer;
	  pGeometry->m_VertexCount = sm->m_iVertexCount;
	  pGeometry->m_VertexSize	= sm->m_iVertexSize;
	  pGeometry->m_pIndexBuffer = sm->m_pIndexBuffer;
	  pGeometry->m_IndexCount = sm->m_iIndexCount;
	  pGeometry->m_PrimitiveTopology = sm->m_PrimitiveTopology;

	  for ( int j = 0, m = mat->GetPassCount(); j<m; ++j )
	  {
		  Pass* pPass = mat->GetPass( j );
			
      //
      //@todo do something smarter with the naming of the input layouts!
      //

      // Attempt to grab the input layout
      InputLayout* pInputLayout = InputLayoutManager::getSingletonPtr()->GetByName( "basic_mesh_layout" );
      if ( pInputLayout == NULL )
      {
        //
        //TODO: Use the vertex description from the sm.
        //
        VertexDeceleration* pVertexDecl = NULL;

        // Was unable to find an existing layout, create a new one.
        pInputLayout = InputLayoutManager::getSingletonPtr()->Create( "basic_mesh_layout", NULL, pPass, pVertexDecl );
        // Load the layout.
        pInputLayout->Load();
      }

		  pSubObj->SetGeometry( j, pGeometry );
		  pSubObj->SetInputLayout( j, pInputLayout );
		  //pSubObj->SetRenderTarget( j, 1, Window::DefaultRenderTarget );
	  }

	  pObj->AddSubObject( pSubObj );
  }

  pObj->Initialize();

  m_AttachedObjects.push_back( pObj );

  return pObj;
}*/