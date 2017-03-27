
#include "SceneObject.h"
#include "SceneNode.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Scene/Geometry.h>


using namespace Nebulae;


int SceneObject::ms_nextIdentifier = 0;


SceneObject::SceneObject( SceneNode* parent, const Material* material ) 
	: m_identifier( ms_nextIdentifier++ )
	, m_node( parent )
  , m_material( material )
  , m_visible( true )
{}


SceneObject::~SceneObject()
{ 
  Clear();
  m_node = NULL; 
}


int        
SceneObject::GetIdentifier() const
{ return m_identifier; }


SceneNode* 
SceneObject::GetNode() const
{ return m_node; }


bool       
SceneObject::IsVisible() const
{ return m_visible; }


const Material* 
SceneObject::GetMaterial() const 
{ return m_material; }


UniformParameters& 
SceneObject::GetUniformParameters()
{ return m_uniforms; }


void
SceneObject::Clear()
{
  for( uint32 i = 0; i < m_passData.size(); ++i ) {
    delete m_passData[i];
  }
  m_passData.clear();
}


bool 
SceneObject::Initialize() 
///
/// Creates a PassData structure for each Material pass for m_material.
///
/// @return
///   true if success else false.
///
{ 
  if( NULL == m_material ) return false;

  //@todo Setup the uniform parameters based upon the Material.
  const UniformDefinitionMap& uniforms = m_material->GetUniformDefinitions();
  
  UniformDefinitionMap::const_iterator i = uniforms.begin();
  for( ; i != uniforms.end(); ++i ) 
	{
    const UniformDefinition& def = i->second;
    m_uniforms.AddUniformDefinition( i->first, def.type, def.arraySize );
  }

  for( std::size_t idx = 0, n = m_material->GetPassCount(); idx < n; ++idx ) 
	{
    PassData* pData = new PassData();
    pData->VertexLayout = nullptr;
    pData->Geometry     = nullptr;
    //data.pDepthStencilView     = Window::ms_pDepthStencilView;
    //data.ppRenderTargetViews   = Window::ms_ppRenderTargetViews;
    //data.RenderTargetViewCount = Window::ms_RenderTargetViewCount;

    m_passData.push_back( pData );
  }

  return true;
}


void
SceneObject::SetVisible( bool bVisible )
{ m_visible = bVisible; }


void 
SceneObject::SetGeometry( std::size_t iPass, Geometry* pGeometry )
{ m_passData[iPass]->Geometry = pGeometry; }


void 
SceneObject::SetInputLayout( std::size_t iPass, InputLayout* pInputLayout ) 
{ m_passData[iPass]->VertexLayout = pInputLayout; }


void
SceneObject::PreRender( Camera* camera )
{
  const UniformDefinitionMap&          uniformMap = m_uniforms.GetUniformDefinitions();
  UniformDefinitionMap::const_iterator it         = uniformMap.begin();
  for( ; it != uniformMap.end(); ++it ) {
    if( it->first.compare( "world" ) == 0 ) {
      Matrix4 worldMatrix;
      worldMatrix.SetIdentity();
      m_node->GetWorldMatrix( &worldMatrix );
      m_uniforms.SetNamedUniform( it->first, worldMatrix.ptr(), 16 );
    } else if( it->first.compare( "view" ) == 0 ) {
      m_uniforms.SetNamedUniform( it->first, camera->GetViewMatrix().ptr(), 16 );
    } else if( it->first.compare( "projection" ) == 0 ) {
      m_uniforms.SetNamedUniform( it->first, camera->GetProjectionMatrix().ptr(), 16 );
    }
  }
}


void
SceneObject::Render( RenderSystemPtr renderDevice ) const
{
  for( std::size_t i = 0, n = m_material->GetPassCount(); i < n; ++i ) 
  {
    Pass* pPass = m_material->GetPass( i );

    // Set the Shaders for this pass.
    renderDevice->SetShaders( pPass->GetVertexShader(), pPass->GetPixelShader() );

    // Set the Vertex Buffer
    std::size_t iOffset = 0;
    std::size_t iStride = 0; //m_passData[i]->Geometry->m_VertexSize;
    HardwareBuffer* pVertexBuffer = m_passData[i]->Geometry->m_vertexBuffer;
    renderDevice->SetVertexBuffers( 0, pVertexBuffer, iStride, iOffset );

    // Set the Vertex input layout.
    renderDevice->SetInputLayout( m_passData[i]->VertexLayout );
      
    // Bind the draw operation type.
    renderDevice->SetOperationType( m_passData[i]->Geometry->m_primitiveTopology );

    // Bind the uniform values.
    const UniformDefinitionMap&          uniformMap = m_uniforms.GetUniformDefinitions();
    UniformDefinitionMap::const_iterator it         = uniformMap.begin();
    for( ; it != uniformMap.end(); ++it ) {
      UniformDefinition def = renderDevice->GetUniformByName( it->first.c_str() );
      if( def.IsFloat() ) {
        renderDevice->SetUniformBinding( def, (void*)m_uniforms.GetFloatPointer(it->second.physicalIndex) );
      } else {
        renderDevice->SetUniformBinding( def, (void*)m_uniforms.GetIntPointer(it->second.physicalIndex) );
      }
    }

    // Draw function
    if( m_passData[i]->Geometry->m_indexBuffer != NULL ) {
      renderDevice->SetIndexBuffer( m_passData[i]->Geometry->m_indexBuffer, 0 );
      renderDevice->DrawIndexed( m_passData[i]->Geometry->m_indexCount, 0, 0 );
    } else {
      renderDevice->Draw( m_passData[i]->Geometry->m_vertexCount, 0 );
    }
  }
}
