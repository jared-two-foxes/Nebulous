
#include "Effect.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>

using Nebulae::Effect;


const Nebulae::HardwareShader* Effect::GetVertexShader() const
{ return m_vertexShader; }

const Nebulae::HardwareShader* Effect::GetPixleShader() const
{ return m_pixelShader; }



Effect::Effect()
: m_vertexShader( nullptr ),
  m_pixelShader( nullptr )
{}


Effect::~Effect()
{
  if( m_vertexShader )
    delete m_vertexShader;
  if( m_pixelShader )
    delete m_pixelShader;
}


void
Effect::Init( RenderSystemPtr renderDevice )
{
  NE_ASSERT( !m_vertexShader, "Init called but vertex shader already exists." )();
  NE_ASSERT( !m_pixelShader, "Init called but pixel shader already exists." )();

  m_vertexShader = renderDevice->CreateShader( "debug_vs.glsl", VERTEX_SHADER );
  m_pixelShader  = renderDevice->CreateShader( "debug_fs.glsl", PIXEL_SHADER );
}


void
Effect::Apply( Effect::RenderSystemPtr renderDevice )
{
  // Set the Shaders for this pass.
  renderDevice->SetShaders( m_vertexShader, m_pixelShader );

  UniformDefinition uWorld = renderDevice->GetUniformByName( "world" );
  //if ( uWorld.IsValid() )
  {
    renderDevice->SetUniformBinding( uWorld, m_world.ptr() );
  }

  UniformDefinition uView = renderDevice->GetUniformByName( "view" );
  //if ( uView.IsValid() )
  {
    renderDevice->SetUniformBinding( uView, m_view.ptr() );
  }

  UniformDefinition uProjection = renderDevice->GetUniformByName( "projection" );
  //if ( uProjection.IsValid() )
  {
    renderDevice->SetUniformBinding( uProjection, m_projection.ptr() );
  }
}


void 
Effect::SetWorld( Matrix4 value )
{
  m_world = value; 
}


void 
Effect::SetView( Matrix4 value )
{
  m_view = value;
}


void 
Effect::SetProjection( Matrix4 value )
{
  m_projection = value;
}


void 
Effect::SetMatrices( Matrix4 world, Matrix4 view, Matrix4 projection )
{
  m_world = world;
  m_view = view;
  m_projection = projection;
}