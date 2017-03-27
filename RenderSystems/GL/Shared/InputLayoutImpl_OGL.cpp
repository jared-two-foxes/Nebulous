
#include "InputLayoutImpl_OGL.h"
#include "ProgramObject.h"

#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>



#define BUFFER_OFFSET(i) ((void*)(i))


PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = NULL;
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation        = NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = NULL;

using namespace Nebulae;

void InputLayoutImpl_OGL::initiateFunctions()
{
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress( "glDisableVertexAttribArray" );
	glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress( "glEnableVertexAttribArray" );
	glGetAttribLocation        = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress( "glGetAttribLocation" );
	glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress( "glVertexAttribPointer" );
}

//constructor
InputLayoutImpl_OGL::InputLayoutImpl_OGL( const VertexDeceleration* vertexDecl, const HardwareShader* pVertexShader )
	: InputLayoutImpl( vertexDecl, pVertexShader )
{ 
}
  
// destructor
InputLayoutImpl_OGL::~InputLayoutImpl_OGL() 
{
}


bool InputLayoutImpl_OGL::Load()
{ return true; }


void InputLayoutImpl_OGL::Bind( ProgramObject* programObject )
{
	GLuint      iHandle = programObject->GetHandle();
	std::size_t iStride = m_vertexDecl->GetVertexSize();
	
  // @note [jared.watt] Vertex Array should already be bound at this point so just
  // setup the input layout.
  GLint iSize     = 4;
  GLenum iFormat  = GL_FLOAT;
	for( int i = 0, n = m_vertexDecl->GetElementCount(); i < n; ++i ) {
		VertexElement* pElement = m_vertexDecl->GetVertexElement( i );

    switch( pElement->Format ) {
    case VET_FLOAT1:
      iFormat = GL_FLOAT;
      iSize = 1;
      break;
    case VET_FLOAT2:
      iFormat = GL_FLOAT;
      iSize = 2;
      break;
    case VET_FLOAT3:
      iFormat = GL_FLOAT;
      iSize = 3;
      break;
    case VET_FLOAT4:
      iFormat = GL_FLOAT;
      iSize = 4;
      break;
    case VET_SHORT1:
      iFormat = GL_SHORT;
      iSize = 1;
      break;
    case VET_SHORT2:
      iFormat = GL_SHORT;
      iSize = 2;
      break;
    case VET_SHORT3:
      iFormat = GL_SHORT;
      iSize = 3;
      break;
    case VET_SHORT4:
      iFormat = GL_SHORT;
      iSize = 4;
      break;
    case VET_UBYTE4:
      iFormat = GL_BYTE;
      iSize = 4;
      break;
    }
    
		if( pElement->Semantic == VES_POSITION ) {
      int iPositionLocation = glGetAttribLocation( iHandle, "a_vertex" );
      if( -1 != iPositionLocation ) {
			  glVertexAttribPointer( iPositionLocation, iSize, iFormat, GL_FALSE, (GLsizei)iStride, BUFFER_OFFSET( pElement->AlignedByteOffset ) );
			  glEnableVertexAttribArray( iPositionLocation );
      }
		} else if( pElement->Semantic == VES_NORMAL ) {
      int iNormalLocation = glGetAttribLocation( iHandle, "a_normal" );
      if( -1 != iNormalLocation ) {
			  glVertexAttribPointer( iNormalLocation, iSize, iFormat, GL_FALSE, (GLsizei)iStride, BUFFER_OFFSET( pElement->AlignedByteOffset ) );
			  glEnableVertexAttribArray( iNormalLocation );
      }
		} else if( pElement->Semantic == VES_TEXTURE_COORDINATES ) {
      int iTexCoordLocation = glGetAttribLocation( iHandle, "a_texcoord" );
      if( -1 != iTexCoordLocation ) {
			  glVertexAttribPointer( iTexCoordLocation, iSize, iFormat, GL_FALSE, (GLsizei)iStride, BUFFER_OFFSET( pElement->AlignedByteOffset ) );
			  glEnableVertexAttribArray( iTexCoordLocation );
      }
		} else if( pElement->Semantic == VES_DIFFUSE ) {
      int iDiffuseLocation = glGetAttribLocation( iHandle, "a_diffuse" );
      if( -1 != iDiffuseLocation )
      {
			  glVertexAttribPointer( iDiffuseLocation, iSize, iFormat, GL_FALSE, (GLsizei)iStride, BUFFER_OFFSET( pElement->AlignedByteOffset ) );
			  glEnableVertexAttribArray( iDiffuseLocation );
      }
		}

		CheckForGLError();
	}
}