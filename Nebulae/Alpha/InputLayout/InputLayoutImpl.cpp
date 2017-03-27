
#include "InputLayoutImpl.h"

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>

using Nebulae::InputLayoutImpl;


InputLayoutImpl::InputLayoutImpl( const Nebulae::VertexDeceleration* vertexDecl, const Nebulae::HardwareShader* vertexShader )
: m_vertexDecl(vertexDecl->Clone()),
  m_vertexShader(vertexShader)
{
  NE_ASSERT( vertexDecl, "VertexDeceleration is null" )();
  NE_ASSERT( vertexShader, "VertexShader is null" )();

  // Make a local copy of the vertex deceleration.
  //if( vertexDecl != nullptr )
  //{
  //  m_vertexDecl = vertexDecl->Clone();  
  //}
}

InputLayoutImpl::~InputLayoutImpl() 
{
  if( m_vertexDecl ) 
  {
    delete m_vertexDecl;
  }
  m_vertexDecl = nullptr;

  m_vertexShader = nullptr;
}

const Nebulae::VertexDeceleration* 
InputLayoutImpl::GetVertexDecleration() const 
{ return m_vertexDecl; }

const Nebulae::HardwareShader* 
InputLayoutImpl::GetVertexShader() const 
{ return m_vertexShader; }

bool 
InputLayoutImpl::Load() 
{ return false; }

bool 
InputLayoutImpl::Unload()
{ return false; }