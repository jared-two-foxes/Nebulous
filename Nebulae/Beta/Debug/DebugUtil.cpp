
#include "DebugUtil.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Material.h>

namespace Nebulae
{
 
#define MAX_DEBUG_LINES 128

DebugUtil::DebugUtil( RenderSystemPtr renderer )
  : m_primitiveCount( 0 ), 
    m_renderSystem( renderer ),
    m_material( NULL ),
    m_vertexBuffer( NULL ),
    m_inputLayout( NULL )
{}

void 
DebugUtil::Init()
{
//
// Setup the material for rendering.
//
  HardwareShader* vertexShader = m_renderSystem->CreateShader( "debug_vs.glsl", VERTEX_SHADER );
  HardwareShader* pixelShader  = m_renderSystem->CreateShader( "debug_fs.glsl", PIXEL_SHADER );

  m_material = new Material( "materials//debug.material" ); 
  Pass* pass = m_material->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );

  const uint32 vertexSize = 7*sizeof(Real);
  const uint32 bufferSize = MAX_DEBUG_LINES * vertexSize;

  m_vertexBuffer = m_renderSystem->FindBufferByName( "DebugBuffer" );
  if( m_vertexBuffer == NULL )
  {
    // Unable to find an existing buffer, create a new one.
    m_vertexBuffer = m_renderSystem->CreateBuffer(
      "DebugBuffer", HBU_DYNAMIC, bufferSize, 
      HBB_VERTEX, (void*)NULL );
  }

  // @todo [jared.watt 25.05.2013] Leaks. Needs to be deleted somewhere.
  VertexDeceleration* pVertexDecl = new VertexDeceleration( 3 );
  pVertexDecl->AddElement( VET_FLOAT3, VES_POSITION, 0 );
  pVertexDecl->AddElement( VET_FLOAT4, VES_DIFFUSE, 0 );

  // Attempt to find a valid input layout
  m_inputLayout = m_renderSystem->FindInputLayoutByUsage( pVertexDecl, vertexShader );
  if( m_inputLayout == NULL )
  {
    // Create the layout based upon the materials pass.
    m_inputLayout = m_renderSystem->CreateInputLayout( "DebugLayout", pVertexDecl, vertexShader );
  } 
  
  delete pVertexDecl; 
}


void
DebugUtil::Reset()
{ m_primitiveCount = 0; }


void 
DebugUtil::AddLine( const Vector4& p1, const Vector4& p2, const Vector4& colour )
{
  struct DebugVertex {
    float position[3];
    float colour[4];
  };

  if( m_primitiveCount < MAX_DEBUG_LINES)
  {
    DebugVertex* pVertexData = static_cast<DebugVertex*>( m_vertexBuffer->Lock(HBL_NORMAL) );
    if( pVertexData )
    {
      // 2 verts per primitive
      DebugVertex* vertex = pVertexData + m_primitiveCount * 2;

      vertex->position[0] = p1.x;
      vertex->position[1] = p1.y;
      vertex->position[2] = p1.z;
      vertex->colour[0]   = colour.x; 
      vertex->colour[1]   = colour.y; 
      vertex->colour[2]   = colour.z; 
      vertex->colour[3]   = 1.f;

      vertex++;

      vertex->position[0] = p2.x;
      vertex->position[1] = p2.y;
      vertex->position[2] = p2.z;
      vertex->colour[0]   = colour.x; 
      vertex->colour[1]   = colour.y; 
      vertex->colour[2]   = colour.z; 
      vertex->colour[3]   = 1.f;

      m_primitiveCount++;

      m_vertexBuffer->Unlock();
    }
  }
}


void 
DebugUtil::AddCircle( const Vector4& position, const Real radius, const Vector4& colour )
{
  static const int CIRCLE_RESOLUTION = 20;

  for( int i = 0; i < CIRCLE_RESOLUTION; ++i )
  {
    float angle0 = i * float(2*M_PI / CIRCLE_RESOLUTION);
    float angle1 = angle0 + float(2*M_PI / CIRCLE_RESOLUTION);
    float x0     = position.x + radius * cos( angle0 );
    float z0     = position.z + radius * sin( angle0 );
    float x1     = position.x + radius * cos( angle1 );
    float z1     = position.z + radius * sin( angle1 );

    AddLine( Vector4(x0, 0, z0, 0), Vector4(x1, 0, z1, 0), colour );
  }
}

void 
DebugUtil::Draw( const CameraPtr& camera )
{
  PROFILE;

  NE_ASSERT( m_material != NULL, "Material is not set for Debug drawing util." )();

  if( m_material )
  {
    // Grab the pass for the material (there is only one).
    Pass* pass = m_material->GetPass( 0 );

    // Set the pass to the render system. 
    m_renderSystem->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

    // Set the Vertex Buffer
    size_t iOffset = 0;
    size_t iStride = 7*sizeof(Real);
    m_renderSystem->SetVertexBuffers( 0, m_vertexBuffer, iStride, iOffset );

    // Set the Vertex input layout
    m_renderSystem->SetInputLayout( m_inputLayout );
    
    // Set the operation type
    m_renderSystem->SetOperationType( OT_LINES );

    // Create projection variable from desc.
    UniformDefinition worldVarDef      = m_renderSystem->GetUniformByName( "world" );
    UniformDefinition viewVarDef       = m_renderSystem->GetUniformByName( "view" );
    UniformDefinition projectionVarDef = m_renderSystem->GetUniformByName( "projection" );

    // Calculate the local transform
    Matrix4 worldMatrix;
    worldMatrix.SetIdentity();

    // Set camera transforms for pass
    m_renderSystem->SetUniformBinding( worldVarDef,      (void*)worldMatrix.ptr() );
    m_renderSystem->SetUniformBinding( viewVarDef,       (void*)camera->GetViewMatrix().ptr() );
    m_renderSystem->SetUniformBinding( projectionVarDef, (void*)camera->GetProjectionMatrix().ptr() );  
    
    // Draw particle.
    m_renderSystem->Draw( m_primitiveCount * 2, 0 );
  }
}

}