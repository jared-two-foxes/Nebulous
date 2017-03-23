
#include "GLES2InputLayoutImpl.h"
#include "ProgramObject.h"

#include <Nebulae/Common/Pixel/PixelFormat.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>


#define BUFFER_OFFSET(i) ((void*)(i))

using Nebulae::GLES2InputLayoutImpl;
using Nebulae::InputLayoutImpl;
using Nebulae::ProgramObject;

GLES2InputLayoutImpl::GLES2InputLayoutImpl( const Nebulae::VertexDeceleration* pVertexDecl, const Nebulae::HardwareShader* pVertexShader ) 
: InputLayoutImpl( pVertexDecl, pVertexShader )
{}
  
GLES2InputLayoutImpl::~GLES2InputLayoutImpl() 
{}

bool GLES2InputLayoutImpl::Load()
{
  return true; 
}

void
GLES2InputLayoutImpl::Unbind()
{
  std::for_each( m_boundIndices.begin(), m_boundIndices.end(), 
    [&]( uint32 index ){ glDisableVertexAttribArray(index); } );

  m_boundIndices.clear();
}

void 
GLES2InputLayoutImpl::Bind( ProgramObject* programObject )
{
//
// Need to disable all currently bound vertex attributes.
//
  Unbind();

  GLuint      iHandle = programObject->GetHandle();
  std::size_t iStride = m_vertexDecl->GetVertexSize();  

//
// The vertex array should already be bound at this point so just
// setup the input layout.
//
  GLint  iSize   = GL_FLOAT;
  GLenum iFormat = 4;
  for( int i = 0, n = m_vertexDecl->GetElementCount(); i < n; ++i ) 
  {
    VertexElement* pElement = m_vertexDecl->GetVertexElement( i );

    switch( pElement->Format ) 
    {
    case VET_FLOAT1:
      iFormat = GL_FLOAT;
      iSize   = 1;
      break;
    case VET_FLOAT2:
      iFormat = GL_FLOAT;
      iSize   = 2;
      break;
    case VET_FLOAT3:
      iFormat = GL_FLOAT;
      iSize   = 3;
      break;
    case VET_FLOAT4:
      iFormat = GL_FLOAT;
      iSize   = 4;
      break;
    case VET_SHORT1:
      iFormat = GL_SHORT;
      iSize   = 1;
      break;
    case VET_SHORT2:
      iFormat = GL_SHORT;
      iSize   = 2;
      break;
    case VET_SHORT3:
      iFormat = GL_SHORT;
      iSize   = 3;
      break;
    case VET_SHORT4:
      iFormat = GL_SHORT;
      iSize   = 4;
      break;
    case VET_UBYTE4:
      iFormat = GL_BYTE;
      iSize   = 4;
      break;
    }
    
    if( pElement->Semantic == VES_POSITION ) 
    {
      BindAttribute( iHandle, "a_vertex", iSize, iFormat, (int32)iStride, pElement->AlignedByteOffset );
    } 
    else if( pElement->Semantic == VES_NORMAL )
    {
      BindAttribute( iHandle, "a_normal", iSize, iFormat, (int32)iStride, pElement->AlignedByteOffset );
    } 
    else if( pElement->Semantic == VES_TEXTURE_COORDINATES ) 
    {
      BindAttribute( iHandle, "a_texcoord", iSize, iFormat, (int32)iStride, pElement->AlignedByteOffset );
    } 
    else if( pElement->Semantic == VES_DIFFUSE ) 
    {
      BindAttribute( iHandle, "a_diffuse", iSize, iFormat, (int32)iStride, pElement->AlignedByteOffset );
    }
  }
}

GLint
GLES2InputLayoutImpl::BindAttribute( GLuint iHandle, const char* attribute_name, GLint iSize, GLenum iFormat, int32 iStride, std::size_t offset )
{
  GLint location = glGetAttribLocation( iHandle, attribute_name );
  if( -1 != location )
  {
    glVertexAttribPointer( location, iSize, iFormat, GL_FALSE, iStride, BUFFER_OFFSET(offset) );
    CheckForGLESError();
    glEnableVertexAttribArray( location );
    CheckForGLESError();
    m_boundIndices.push_back( location );
  }

  return location;
}