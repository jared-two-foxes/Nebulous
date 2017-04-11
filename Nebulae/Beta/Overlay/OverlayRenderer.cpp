
#include "OverlayRenderer.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>


#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/SpriteBatch/SpriteBatch.h>

using namespace Nebulae;


const float g_fRectVertices[] = { 0.0f,0.0f, 1.0f,0.0f, 0.0,1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f };


OverlayRenderer::OverlayRenderer( RenderSystemPtr renderSystem, SpriteBatch* batcher )
: m_renderDevice( renderSystem ), 
  m_basicMaterial( nullptr ),
  m_complexMaterial( nullptr ),
  m_batcher( batcher )
{}

bool 
OverlayRenderer::Init()
{
  NE_ASSERT( m_renderDevice, "Valid RenderDevice is not set for the OverlayRenderer Initiation function" )();

  if( !m_renderDevice || !m_renderDevice->IsInitialized() ) {
    return false;
  }

//
// Setup the material for rendering gui components.
//
  HardwareShader* vertexShader = m_renderDevice->FindShaderByName( "gui_quad_vs.glsl" );
  if( NULL == vertexShader ) {
    vertexShader = m_renderDevice->CreateShader( "gui_quad_vs.glsl", VERTEX_SHADER );
  }
  HardwareShader* pixelShader = m_renderDevice->FindShaderByName( "gui_quad_fs.glsl" );
  if( NULL == pixelShader ) {
    pixelShader = m_renderDevice->CreateShader( "gui_quad_fs.glsl", PIXEL_SHADER );
  }


  m_basicMaterial = new Material( "materials//gui_quad.material" );
  Pass* pass = m_basicMaterial->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );
  
  vertexShader = m_renderDevice->FindShaderByName( "gui_complex_quad_vs.glsl" );
  if( NULL == vertexShader ) {
    vertexShader = m_renderDevice->CreateShader( "gui_complex_quad_vs.glsl", VERTEX_SHADER );
  }
    
  m_complexMaterial = new Material( "materials//gui_quad.material" ); 
  pass = m_complexMaterial->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );


//
// Create the buffers.
//
  HardwareBuffer* pBuffer = m_renderDevice->FindBufferByName( "GuiVertexBuffer" );
  if( pBuffer == NULL ) {
    pBuffer = m_renderDevice->CreateBuffer(
      "GuiVertexBuffer", HBU_STATIC_WRITE_ONLY, 12*sizeof(float),
      HBB_VERTEX, (void*)&g_fRectVertices[0] );
  }

  pBuffer = m_renderDevice->FindBufferByName( "QuadBuffer" );
  if( pBuffer == NULL ) {
    pBuffer = m_renderDevice->CreateBuffer(
      "QuadBuffer", HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, 12*sizeof(float),
      HBB_VERTEX, NULL );
  }

  VertexDeceleration* pVertexDecl = new VertexDeceleration( 2 );
  pVertexDecl->AddElement( VET_FLOAT2, VES_POSITION, 0 );

//
// Attempt to grab the input layout
//
  InputLayout* pInputLayout = m_renderDevice->FindInputLayoutByUsage( pVertexDecl, vertexShader );
  if( pInputLayout == NULL ) {
    // Create the layout based upon the materials first pass.
    pInputLayout = m_renderDevice->CreateInputLayout( "BasicGuiLayout", pVertexDecl, vertexShader );
  }

  delete pVertexDecl;

  return ((m_basicMaterial != NULL) && (m_complexMaterial != NULL) && (pBuffer != NULL) && (pInputLayout != NULL));
}

void
OverlayRenderer::DrawQuad( RenderSystemPtr renderer, const Nebulae::Point& upperLeft, const Nebulae::Point& lowerRight, 
                        const Colour& colour, const SubTexture* subtexture, float depth, float rotation ) const
{
  if ( m_batcher )
  {
    m_batcher->AddQuad( upperLeft, lowerRight, colour, subtexture, depth, rotation );
  }
  else
  {
    NE_ASSERT( renderer, "Null renderer passed to RenderWidget" )();
    NE_ASSERT( m_basicMaterial->GetPassCount() == 1, "Material is expected to have a single pass." )();

    // Early out if renderer is not valid.
    if( !renderer ) {
      return;
    }

  //
  // Bind the material pass.
  //
    Pass* pass = m_basicMaterial->GetPass( 0 );
    if( NULL == pass ) {
      return;
    }

    renderer->SetOperationType( OT_TRIANGLES );
    renderer->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );
  

  //
  // Bind the Vertex Buffer
  //
    size_t          offset = 0;
    size_t          stride = 2*sizeof(float);
    HardwareBuffer* buffer = renderer->FindBufferByName( "GuiVertexBuffer" );
    if( buffer ) {
      renderer->SetVertexBuffers( 0, buffer, stride, offset );
    }


  //
  // Bind the vertex input layout.
  //
    InputLayout* inputLayout = renderer->FindInputLayoutByName( "BasicGuiLayout" );
    if( inputLayout ) {
      renderer->SetInputLayout( inputLayout );
    }


  //
  // Create the uniform values for current pass.
  //
    const RenderSystem::WindowPtr window            = renderer->GetWindow();
    const int                     windowHeight      = window->GetHeight();
    const Real                    colourModifier    = 1 / 255.f;
    const Real                    offsetBuffer[2]   = { Real(upperLeft.x), Real(windowHeight) - Real(lowerRight.y) }; //< ll position in render projection.  
    const Real                    sizeBuffer[2]     = { Real(lowerRight.x) - Real(upperLeft.x), Real(lowerRight.y) - Real(upperLeft.y) };
    const Real                    colourBuffer[4]   = { colour.r * colourModifier, colour.g * colourModifier, colour.b * colourModifier, colour.a * colourModifier };
    Real                          texCoordBuffer[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

    UniformDefinition offsetVarDef   = renderer->GetUniformByName( "offset" );
    UniformDefinition sizeVarDef     = renderer->GetUniformByName( "size" );
    UniformDefinition angleVarDef    = renderer->GetUniformByName( "angle" );
    UniformDefinition diffuseVarDef  = renderer->GetUniformByName( "diffuseTexture" );
    UniformDefinition colourVarDef   = renderer->GetUniformByName( "colour" );
    UniformDefinition texCoordVarDef = renderer->GetUniformByName( "texcoord" );
    UniformDefinition depthVarDef    = renderer->GetUniformByName( "depth" );

    int32 identifier = -1;
    if( subtexture ) 
    {
      identifier = subtexture->GetTexture()->GetIdentifier();

      Vector2 minCoord = subtexture->GetMinCoord();
      Vector2 maxCoord = subtexture->GetMaxCoord();

      texCoordBuffer[0] = minCoord.x;
      texCoordBuffer[1] = minCoord.y;
      texCoordBuffer[2] = maxCoord.x;
      texCoordBuffer[3] = maxCoord.y;
    }
    else 
    {
      Texture* defaultWhiteTexture = renderer->FindTextureByName( "white.png" );
      if( !defaultWhiteTexture ) 
      {
        defaultWhiteTexture = renderer->CreateTexture( "white.png" );
      }
      identifier = defaultWhiteTexture->GetIdentifier();
    }

    renderer->SetUniformBinding( offsetVarDef,   (void*)&offsetBuffer[0] );
    renderer->SetUniformBinding( sizeVarDef,     (void*)&sizeBuffer[0] );
    renderer->SetUniformBinding( angleVarDef,    (void*)&rotation );
    renderer->SetUniformBinding( colourVarDef,   (void*)&colourBuffer[0] );
    renderer->SetUniformBinding( diffuseVarDef,  (void*)&identifier );
    renderer->SetUniformBinding( texCoordVarDef, (void*)&texCoordBuffer[0] );
    renderer->SetUniformBinding( depthVarDef,    (void*)&depth );

  //
  // Draw geometry.
  //
    renderer->Draw( 6, 0 );
  }
}

void
OverlayRenderer::DrawComplexQuad( RenderSystemPtr renderer, const Nebulae::Point& ul, const Nebulae::Point& ur,
                               const Nebulae::Point& ll, const Nebulae::Point& lr, const Colour& colour,
                               const SubTexture* subtexture, float depth, float rotation) const
{
  // Early out if renderer is not valid.
  NE_ASSERT( renderer, "Null renderer passed to RenderWidget" )();

  if( !renderer ) {
    return;
  }

  const RenderSystem::WindowPtr window       = renderer->GetWindow();
  const int32                   windowHeight = window->GetHeight();

  const Nebulae::Point ul_( ul.x, windowHeight - ul.y );
  const Nebulae::Point ur_( ur.x, windowHeight - ur.y );
  const Nebulae::Point ll_( ll.x, windowHeight - ll.y );
  const Nebulae::Point lr_( lr.x, windowHeight - lr.y );


//
// Bind the material pass.
//
  NE_ASSERT( m_complexMaterial->GetPassCount() == 1, "Invalid number of passes found?" )();
  NE_ASSERT( m_complexMaterial->GetPass( 0 ) != NULL, "Unable to find a pass for binding?" )();
    
  Pass* pass = m_complexMaterial->GetPass( 0 );
  if( NULL == pass ) {
    return;
  }

  renderer->SetOperationType( OT_TRIANGLES );
  renderer->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );


//
// Bind the Vertex Buffer
//
  size_t          offset = 0;
  size_t          stride = 2*sizeof(float);
  HardwareBuffer* buffer = renderer->FindBufferByName( "GuiVertexBuffer" );
  if( buffer ) {
    renderer->SetVertexBuffers( 0, buffer, stride, offset );
  }


//
// Bind the vertex input layout.
//
  InputLayout* inputLayout = renderer->FindInputLayoutByName( "BasicGuiLayout" );
  if( inputLayout ) {
    renderer->SetInputLayout( inputLayout );
  }
  
//
// Create the uniform values for current pass.
//
  UniformDefinition leftVarDef     = renderer->GetUniformByName( "left" );
  UniformDefinition rightVarDef     = renderer->GetUniformByName( "right" );
  UniformDefinition diffuseVarDef  = renderer->GetUniformByName( "diffuseTexture" );
  UniformDefinition colourVarDef   = renderer->GetUniformByName( "colour" );
  UniformDefinition texCoordVarDef = renderer->GetUniformByName( "texcoord" );
  UniformDefinition depthVarDef    = renderer->GetUniformByName( "depth" );
    


  const Real colourModifier    = 1 / 255.f;
  const Real colourBuffer[4]   = { colour.r * colourModifier, colour.g * colourModifier, colour.b * colourModifier, colour.a * colourModifier };
  const Real left_coords[4]    = { Real(ll_.x), Real(ll_.y), Real(ul_.x), Real(ul_.y) }; 
  const Real right_coords[4]   = { Real(lr_.x), Real(lr_.y), Real(ur_.x), Real(ur_.y) }; 
  Real       texCoordBuffer[4] = { 0, 0, 1.0f, 1.0f };


//
// Set the uniform variables for current pass.
//
  int32 identifier = -1;
  if( subtexture ) {
    identifier = subtexture->GetTexture()->GetIdentifier();

    Vector2 minCoord = subtexture->GetMinCoord();
    Vector2 maxCoord = subtexture->GetMaxCoord();

    texCoordBuffer[0] = minCoord.x;
    texCoordBuffer[1] = minCoord.y;
    texCoordBuffer[2] = maxCoord.x;
    texCoordBuffer[3] = maxCoord.y;
  } else {
    Texture* defaultWhiteTexture = renderer->FindTextureByName( "white.png" );
    if( !defaultWhiteTexture ) {
      defaultWhiteTexture = renderer->CreateTexture( "white.png" );
    }
    identifier = defaultWhiteTexture->GetIdentifier();
  }

  renderer->SetUniformBinding( leftVarDef,     (void*)&left_coords[0] );
  renderer->SetUniformBinding( rightVarDef,     (void*)&right_coords[0] );
  renderer->SetUniformBinding( colourVarDef,   (void*)&colourBuffer[0] );
  renderer->SetUniformBinding( diffuseVarDef,  (void*)&identifier );
  renderer->SetUniformBinding( texCoordVarDef, (void*)&texCoordBuffer[0] );
  renderer->SetUniformBinding( depthVarDef,    (void*)&depth );

//
// Draw geometry.
//
  renderer->Draw( 6, 0 );
}

void 
OverlayRenderer::Rectangle( RenderSystemPtr renderer, Nebulae::Point ul, Nebulae::Point lr, Colour colour,
                           Colour borderColour1, Colour borderColour2, uint32 bevelThick, bool bevelLeft,
                           bool bevelTop, bool bevelRight, bool bevelBottom ) const
{
  int inner_x1 = ul.x + (bevelLeft ? static_cast<int>(bevelThick) : 0);
  int inner_y1 = ul.y + (bevelTop ? static_cast<int>(bevelThick) : 0);
  int inner_x2 = lr.x - (bevelRight ? static_cast<int>(bevelThick) : 0);
  int inner_y2 = lr.y - (bevelBottom ? static_cast<int>(bevelThick) : 0);

  const Nebulae::Point ur(lr.x, ul.y);
  const Nebulae::Point ll(ul.x, lr.y);
  
  const Nebulae::Point inner_ul(inner_x1, inner_y1);
  const Nebulae::Point inner_ur(inner_x2, inner_y1);
  const Nebulae::Point inner_ll(inner_x1, inner_y2);
  const Nebulae::Point inner_lr(inner_x2, inner_y2);
  
  // draw beveled edges
  if( bevelThick && (borderColour1 != CLR_ZERO || borderColour2 != CLR_ZERO) ) 
  {
    DrawComplexQuad( renderer, ul, ur, inner_ul, inner_ur, borderColour1 );
    DrawComplexQuad( renderer, ul, inner_ul, ll, inner_ll, borderColour1 );
    DrawComplexQuad( renderer, inner_ll, inner_lr, ll, lr, borderColour2 );
    DrawComplexQuad( renderer, inner_ur, ur, inner_lr, lr, borderColour2 );
  }

  // draw interior of rectangle
  DrawQuad( renderer, inner_ul, inner_lr, colour );
}

void 
OverlayRenderer::FlatRectangle( Point ul, Point lr, Colour color, Colour borderColor, uint32 borderThick ) const
{ 
  Rectangle( m_renderDevice, ul, lr, color, borderColor, borderColor, borderThick, true, true, true, true ); 
}

void 
OverlayRenderer::BeveledRectangle( Point ul, Point lr, Colour color, Colour borderColor, bool up, uint32 bevelThick,
                                  bool bevelLeft, bool bevelTop, bool bevelRight, bool bevelBottom ) const
{
  Rectangle( m_renderDevice, ul, lr, color,
             (up ? GetLightColour(borderColor) : GetDarkColour(borderColor)),
             (up ? GetDarkColour(borderColor) : GetLightColour(borderColor)),
             bevelThick, bevelLeft, bevelTop, bevelRight, bevelBottom);
}

Colour
OverlayRenderer::GetLightColour( Colour clr ) const
{
  const double scale_factor = 2.0;   // factor by which the color is lightened
  Colour retval = clr;
  retval.r = std::min(static_cast<int>(retval.r * scale_factor), 255);
  retval.g = std::min(static_cast<int>(retval.g * scale_factor), 255);
  retval.b = std::min(static_cast<int>(retval.b * scale_factor), 255);
  return retval;
}

Colour 
OverlayRenderer::GetDarkColour( Colour clr ) const
{
  const double scale_factor = 2.0;   // factor by which the color is darkened
  Colour retval = clr;
  retval.r = static_cast<int>(retval.r / scale_factor);
  retval.g = static_cast<int>(retval.g / scale_factor);
  retval.b = static_cast<int>(retval.b / scale_factor);
  return retval;
}

Colour 
OverlayRenderer::GetDisabledColour( Colour clr ) const
{
  Colour retval = clr;
  const double gray_factor = 0.75; // amount to move clr in the direction of gray
  retval.r = static_cast<int>(retval.r + (CLR_GRAY.r - retval.r) * gray_factor);
  retval.g = static_cast<int>(retval.g + (CLR_GRAY.g - retval.g) * gray_factor);
  retval.b = static_cast<int>(retval.b + (CLR_GRAY.b - retval.b) * gray_factor);
  return retval;
}