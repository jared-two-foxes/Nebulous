
#include "SpriteBatch.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Material/Material.h>


using namespace Nebulae;


SpriteBatch::SpriteBatch( const RenderSystemPtr& renderDevice )
: m_renderDevice(renderDevice)
{}

bool 
SpriteBatch::Init()
{
  NE_ASSERT( m_renderDevice, "Valid RenderDevice is not set for the SpriteBatch Init function" )();

  if( !m_renderDevice || !m_renderDevice->IsInitialized() ) 
  {
    return false;
  }

//
// Setup the material for rendering gui components.
//
  HardwareShader* vertexShader = m_renderDevice->FindShaderByName( "batch_vs.glsl" );
  if( nullptr == vertexShader ) 
  {
    vertexShader = m_renderDevice->CreateShader( "batch_vs.glsl", VERTEX_SHADER );
  }
  
  HardwareShader* pixelShader = m_renderDevice->FindShaderByName( "batch_fs.glsl" );
  if( nullptr == pixelShader ) 
  {
    pixelShader = m_renderDevice->CreateShader( "batch_fs.glsl", PIXEL_SHADER );
  }


  m_material = new Material( "materials//batch.material" );
  Pass* pass = m_material->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );
  

  

//
// Create the buffers.
//
  HardwareBuffer* pBuffer = m_renderDevice->FindBufferByName( "BatchBuffer" );
  if( pBuffer == nullptr ) 
  {
    pBuffer = m_renderDevice->CreateBuffer(
      "BatchBuffer", HBU_DYNAMIC_WRITE_ONLY, MAX_VERTICES_PER_BUFFER * sizeof(QuadVertex),
      HBB_VERTEX, nullptr );
  }


//
// Attempt to grab the input layout
//
  VertexDeceleration* pVertexDecl = new VertexDeceleration( 4 );
  pVertexDecl->AddElement( VET_FLOAT4, VES_POSITION, 0 );
  pVertexDecl->AddElement( VET_FLOAT4, VES_DIFFUSE, 0 );
  pVertexDecl->AddElement( VET_FLOAT4, VES_TEXTURE_COORDINATES, 0 );

  InputLayout* pInputLayout = m_renderDevice->FindInputLayoutByUsage( pVertexDecl, vertexShader );
  if( pInputLayout == nullptr ) 
  {
    pInputLayout = m_renderDevice->CreateInputLayout( "BatchLayout", pVertexDecl, vertexShader );
  }

  delete pVertexDecl;

  return ((m_material != nullptr) && (pBuffer != nullptr) && (pInputLayout != nullptr));
}


void
SpriteBatch::AddQuad( GlyphState* state, std::size_t count )
{
  for( std::size_t i = 0; i < count; ++i )
  {
		if ( !state[i].sub_texture )
			continue;

    const Texture* texture = state[i].sub_texture->GetTexture();

    auto it = m_quads.find( const_cast<Texture* >(texture) );
    if( it == m_quads.end() )
    {
      // Push new VertexList pair into array.
      auto result = m_quads.insert( std::make_pair( const_cast<Texture* >(texture), VertexArray() ) );
      NE_ASSERT( result.second, "Failed to insert List for texture." )();
      it = result.first;
    }

    VertexArray& array = (*it).second;
    if( array.remaining < 6 ) //< magic number 6 since each quad has 6 vertices.
    {
      // Have to flush all the arrays.
      Draw();
    }

    const RenderSystem::WindowPtr window            = m_renderDevice->GetWindow();
    const int                     windowHeight      = window->GetHeight();
    
    float w  = Real(state[i].lowerRight.x) - Real(state[i].upperLeft.x);
    float h  = Real(state[i].lowerRight.y) - Real(state[i].upperLeft.y);
    float x1 = Real(state[i].upperLeft.x); 
    float y1 = Real(windowHeight) - Real(state[i].lowerRight.y); 
    float x2 = x1 + w;
    float y2 = y1 + h;

    Vector2 minCoord = state[i].sub_texture->GetMinCoord();
    Vector2 maxCoord = state[i].sub_texture->GetMaxCoord();

    float tx1 = minCoord.x;
    float ty1 = minCoord.y;
    float tx2 = maxCoord.x;
    float ty2 = maxCoord.y;

    //todo: Handle case where Vertex fails to add due to exceeding past count bounds. 
    array.AddVertex( QuadVertex(Point(x1,y1), state[i].depth, state[i].colour, Vector4(tx1,ty1,0,0)) );
    array.AddVertex( QuadVertex(Point(x2,y1), state[i].depth, state[i].colour, Vector4(tx2,ty1,0,0)) );
    array.AddVertex( QuadVertex(Point(x1,y2), state[i].depth, state[i].colour, Vector4(tx1,ty2,0,0)) );
    array.AddVertex( QuadVertex(Point(x2,y1), state[i].depth, state[i].colour, Vector4(tx2,ty1,0,0)) );
    array.AddVertex( QuadVertex(Point(x2,y2), state[i].depth, state[i].colour, Vector4(tx2,ty2,0,0)) );
    array.AddVertex( QuadVertex(Point(x1,y2), state[i].depth, state[i].colour, Vector4(tx1,ty2,0,0)) );
  }
}

void 
SpriteBatch::AddQuad( const Nebulae::Point& upperLeft, const Nebulae::Point& lowerRight,
  const Colour& colour, const SubTexture* texture, Real depth, Real rotation )
{
  GlyphState state(upperLeft, lowerRight);
  state.colour = colour;
  state.sub_texture = texture;
  state.depth = depth;
  state.rotation = rotation;

  AddQuad( &state, 1 );
}

void
SpriteBatch::Draw()
{
  NE_ASSERT( m_renderDevice, "Null renderer passed to RenderWidget" )();
  NE_ASSERT( m_material->GetPassCount() == 1, "Material is expected to have a single pass." )();

  size_t          offset      = 0;
  size_t          stride      = sizeof(QuadVertex);
  HardwareBuffer* buffer      = m_renderDevice->FindBufferByName( "BatchBuffer" );
  InputLayout*    inputLayout = m_renderDevice->FindInputLayoutByName( "BatchLayout" );

//
// Bind the material pass.
//
  Pass* pass = m_material->GetPass( 0 );
  if( NULL == pass ) {
    return;
  }

  m_renderDevice->SetOperationType( OT_TRIANGLES );

  m_renderDevice->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

  const RenderSystem::WindowPtr window = m_renderDevice->GetWindow();
  NE_ASSERT( window, "Unable to retrieve the window object that the RenderDevice is bound to." )();
  
  const Real fLeft   = 0.0f;
  const Real fRight  = (Real)window->GetWidth();
  const Real fBottom = 0.0f;
  const Real fTop    = (Real)window->GetHeight();

  // Setup bounds that we wish to be visible.
  const Real fNear   = 0.001f;
  const Real fFar    = 1000.0f;

  // Create the ortho matrix.
  Matrix4 projection;
  projection.MakeOrthoMatrix( fLeft, fRight, fBottom, fTop, fNear, fFar );

  // Grab the material that we are using.
  UniformDefinition projectionDef = m_renderDevice->GetUniformByName( "projection" );
  m_renderDevice->SetUniformBinding( projectionDef, projection.ptr() );


  for( auto it = m_quads.begin(); it != m_quads.end(); ++it )
  {
    VertexArray& array = it->second;
    if( array.GetUsed() == 0 ) continue; 
  
  //
  // Create the uniform values for current pass.
  //
    UniformDefinition diffuseVarDef  = m_renderDevice->GetUniformByName( "diffuseTexture" );
    NE_ASSERT( it->first, "VertexList is not associated with a valid Texture" )();
    int32 identifier = it->first->GetIdentifier();
    m_renderDevice->SetUniformBinding( diffuseVarDef,  (void*)&identifier );
  

  //
  // Push data to buffer.
  //
    std::size_t length = MAX_VERTICES_PER_BUFFER * sizeof(QuadVertex);
    const QuadVertex* source = array.vertices.data();
    if( buffer )
    {
      buffer->WriteData( 0, length, source, true );
    }
    

  //
  // Bind the Vertex Buffer
  //
    if( buffer ) 
    {
      m_renderDevice->SetVertexBuffers( 0, buffer, stride, offset );
    }

    
  //
  // Bind the vertex input layout.
  //
    if( inputLayout ) 
    {
      m_renderDevice->SetInputLayout( inputLayout );
    }
  

  //
  // Draw the geometry.
  //
    m_renderDevice->Draw( array.GetUsed(), 0 );
  
    array.Clear();
  }
}