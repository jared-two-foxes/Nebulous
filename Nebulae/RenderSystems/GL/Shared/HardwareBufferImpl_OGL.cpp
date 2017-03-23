
#include "HardwareBufferImpl_OGL.h"

PFNGLBINDBUFFERPROC    glBindBuffer    = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLGENBUFFERSPROC    glGenBuffers    = NULL;
PFNGLBUFFERDATAPROC    glBufferData    = NULL;
PFNGLMAPBUFFERPROC     glMapBuffer     = NULL;
PFNGLUNMAPBUFFERPROC   glUnmapBuffer   = NULL;

using namespace Nebulae;

void HardwareBufferImpl_OGL::initiateFunctions()
{
	glBindBuffer    = (PFNGLBINDBUFFERPROC)wglGetProcAddress( "glBindBuffer" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress( "glDeleteBuffers" );
	glGenBuffers    = (PFNGLGENBUFFERSPROC)wglGetProcAddress( "glGenBuffers" );
	glBufferData    = (PFNGLBUFFERDATAPROC)wglGetProcAddress( "glBufferData" );
  glMapBuffer     = (PFNGLMAPBUFFERPROC)wglGetProcAddress( "glMapBuffer" );
  glUnmapBuffer   = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress( "glUnmapBuffer" );
}


GLenum GetGLUsage( const Flags<HardwareBufferUsage>& usage )
{
  if( (usage & HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE) == HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE )
  {  
    return GL_STREAM_DRAW;
  }
  else if( (usage & HBU_DYNAMIC) == HBU_DYNAMIC /*|| (usage & HBU_DYNAMIC_WRITE_ONLY) == HBU_DYNAMIC_WRITE_ONLY*/ )
  {
    return GL_DYNAMIC_DRAW;
  }
  else if( (usage & HBU_STATIC) == HBU_STATIC /*|| (usage & HBU_STATIC_WRITE_ONLY) == HBU_STATIC_WRITE_ONLY*/ )
  {
    return GL_STATIC_DRAW;;
  }
  
  return GL_DYNAMIC_DRAW;
}


HardwareBufferImpl_OGL::HardwareBufferImpl_OGL( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
	:	HardwareBufferImpl( usage, sizeInBytes, bindFlags, sysMem ),
    m_iHandle( 0 ), 
    m_iOpenGlBindFlags( 0 )
{}


HardwareBufferImpl_OGL::~HardwareBufferImpl_OGL()
{
	if( m_iHandle != 0 ) {
		glDeleteBuffers( 1, &m_iHandle );
		m_iHandle = 0;
	}
}


void
HardwareBufferImpl_OGL::Bind()
{
	// Bind the buffer to the appropriate buffer slot.
	glBindBuffer( m_iOpenGlBindFlags, m_iHandle );
	CheckForGLError();
}


bool
HardwareBufferImpl_OGL::Load()
{	
	m_iOpenGlUsage = GetGLUsage( m_Usage );

	// Convert m_Usage into the correct OpenGL equivalent.
	m_iOpenGlBindFlags = GL_ARRAY_BUFFER;
	if( m_BindFlags == HBB_INDEX )
	{
		m_iOpenGlBindFlags = GL_ELEMENT_ARRAY_BUFFER;
	}
	else if( m_BindFlags == HBB_CONSTANT )
	{
		m_iOpenGlBindFlags = GL_UNIFORM_BUFFER;
	}

	glGenBuffers( 1, &m_iHandle );
	CheckForGLError();

	WriteData( 0, m_SizeInBytes, m_SysMem, false );

	return true;
}


void
HardwareBufferImpl_OGL::WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer )
{
	Bind();

	const void* pBuffer = static_cast<const unsigned char*>(pSource) + offset;

	// Load the data into the buffer.
	glBufferData( m_iOpenGlBindFlags, length, pBuffer, m_iOpenGlUsage ); //< this could be using glBufferSubData for existing buffers?
	CheckForGLError();
}


void*
HardwareBufferImpl_OGL::Lock( std::size_t offset, size_t length, LockOptions options )
{
	void* retptr = nullptr;

#if WIN32
	Bind();
	
	GLenum access = 0;
	if( (options & HBL_DISCARD) == HBL_DISCARD )
	{
		// Discard the buffer
		glBufferData( GL_ARRAY_BUFFER, m_SizeInBytes, NULL, GetGLUsage(m_Usage) );
	  CheckForGLError();
	}
	
	if( (m_Usage & HBU_WRITE_ONLY) == HBU_WRITE_ONLY )
	{
	  access = GL_WRITE_ONLY;
	}
	else if( options == HBL_READ_ONLY )
	{
	  access = GL_READ_ONLY;
	}
	else
	{
	  access = GL_READ_WRITE;
	}
	
	void* pBuffer = glMapBuffer( GL_ARRAY_BUFFER, access );
	CheckForGLError();
	
	NE_ASSERT( pBuffer != nullptr, "Vertex Buffer: Out of memory" )();
		
	// return offsetted data
	retptr = static_cast<void*>(
		static_cast<unsigned char*>(pBuffer) + offset);
#endif

	//@todo - throw an exception on iOS due to unsupported function?

	return retptr;
}


void
HardwareBufferImpl_OGL::Unlock()
{
	Bind();

  //let buffer know that we are done modifying data.
	glUnmapBuffer( m_iOpenGlBindFlags );

  CheckForGLError();
}