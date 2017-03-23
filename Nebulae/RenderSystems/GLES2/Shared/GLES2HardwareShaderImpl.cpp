
#include "GLES2HardwareShaderImpl.h"

#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Texture/Texture.h>


using namespace Nebulae;


// A boring utility function which opens a file and extracts the contents.
char* 
file_contents( File& file, GLint *iLength )
{
  char* szBuffer;

  file.SeekToEnd();
  (*iLength) = (int)file.Tell(); 
  file.Seek( 0 );

  szBuffer = new char[ *iLength+1 ];
  file.Read( szBuffer, *iLength );
  szBuffer[ *iLength ] = '\0';

  return szBuffer;
}

//constructor
GLES2HardwareShaderImpl::GLES2HardwareShaderImpl( const std::string& strFileName, HardwareShaderType eType  )
	: HardwareShaderImpl( strFileName, eType )
	, m_iHandle( 0 )
{

}

//destructor
GLES2HardwareShaderImpl::~GLES2HardwareShaderImpl()
{

}



bool 
GLES2HardwareShaderImpl::Load( File& is )
{
//
// Initial Setup.
//
  GLenum type = m_type == PIXEL_SHADER ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;

  m_iHandle = glCreateShader( type );
  CheckForGLESError();

//
// Apply the Shader source.
//
  GLint iFileLen;
  char* szFileContents = file_contents( is, &iFileLen );

  glShaderSource( m_iHandle, 1, (const GLchar**)&szFileContents, NULL );
  CheckForGLESError();

  delete [] szFileContents;


//
// Compile the shader source.
//
  glCompileShader( m_iHandle );
  CheckForGLESError();

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
    CheckForGLESError();

    return false;
  }

  return true;
}