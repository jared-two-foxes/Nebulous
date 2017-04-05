#include "HardwareShaderImpl_OGL.h"

#include <Nebulae/Common/FileSystem/FileSystem.h>
#include <Nebulae/Alpha/Texture/Texture.h>


PFNGLCOMPILESHADERPROC      glCompileShader      = NULL;
PFNGLCREATESHADERPROC       glCreateShader       = NULL;
PFNGLDELETESHADERPROC       glDeleteShader       = NULL;
PFNGLGETSHADERIVPROC        glGetShaderiv        = NULL;
PFNGLGETSHADERINFOLOGPROC   glGetShaderInfoLog   = NULL;
PFNGLSHADERSOURCEPROC       glShaderSource       = NULL;

using namespace Nebulae;


// A boring utility function which opens a file and extracts the contents.
char* 
file_contents( File& file, GLint *iLength )
{
	char* szBuffer;

	file.SeekToEnd();
	(*iLength) = (GLint)file.Tell(); 
	file.Seek( 0 );

	szBuffer = new char[ *iLength+1 ];
	file.Read( szBuffer, *iLength );
	szBuffer[ *iLength ] = '\0';

	return szBuffer;
}


void
HardwareShaderImpl_OGL::initiateFunctions()
{
  glCompileShader    = (PFNGLCOMPILESHADERPROC)wglGetProcAddress( "glCompileShader" );
  glCreateShader     = (PFNGLCREATESHADERPROC)wglGetProcAddress( "glCreateShader" );
  glDeleteShader     = (PFNGLDELETESHADERPROC)wglGetProcAddress( "glDeleteShader" );
  glGetShaderiv      = (PFNGLGETSHADERIVPROC)wglGetProcAddress( "glGetShaderiv" );
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress( "glGetShaderInfoLog" );
  glShaderSource     = (PFNGLSHADERSOURCEPROC)wglGetProcAddress( "glShaderSource" );
}

HardwareShaderImpl_OGL::HardwareShaderImpl_OGL( const std::string& strFileName, HardwareShaderType eType  )
  : HardwareShaderImpl( strFileName, eType )
  , m_iHandle( 0 )
{}

  
HardwareShaderImpl_OGL::~HardwareShaderImpl_OGL()
{}


bool 
HardwareShaderImpl_OGL::Load( File& is )
///
/// @todo [jared.watt 13.02.2014]
///   This will only cater for Vertex and Fragment shaders at this point.
///
{
//
// Initial Setup.
//
  GLenum type = m_type == PIXEL_SHADER ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;

  m_iHandle = glCreateShader( type );
	CheckForGLError();

//
// Apply the Shader source.
//
  GLint iFileLen;
  char* szFileContents = file_contents( is, &iFileLen );
  glShaderSource( m_iHandle, 1, (const GLchar**)&szFileContents, NULL );
	CheckForGLError();
	  
  delete [] szFileContents;


//
// Compile the shader source.
//
  glCompileShader( m_iHandle );
	CheckForGLError();

  GLint iStatus;
  glGetShaderiv( m_iHandle, GL_COMPILE_STATUS, &iStatus );
  if( iStatus == GL_FALSE ) {
    fprintf( stderr, "Failed to compile %s:\n", m_fileName.c_str() );

    GLint iLen;
    glGetShaderiv( m_iHandle, GL_INFO_LOG_LENGTH, &iLen );

    char* szLog = new char[iLen];
    glGetShaderInfoLog( m_iHandle, iLen, NULL, szLog );
    fprintf( stderr, "%s", szLog );
    delete [] szLog;

    glDeleteShader( m_iHandle );

    return false;
  }

  return true;
}
