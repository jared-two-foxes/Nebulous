
#include "GLES2HardwareBufferImpl.h"


using namespace Nebulae;


GLES2HardwareBufferImpl::GLES2HardwareBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
	:	HardwareBufferImpl( usage, sizeInBytes, bindFlags, sysMem ),
    m_iHandle( 0 ),
    m_iOpenGlBindFlags( 0 ),
    m_iOpenGlUsage( 0 )
{}

GLES2HardwareBufferImpl::~GLES2HardwareBufferImpl()
{
	if( m_iHandle != 0 )
	{
		glDeleteBuffers( 1, &m_iHandle );
    CheckForGLESError();

		m_iHandle = 0;
	}
}

void
GLES2HardwareBufferImpl::Bind()
{
	// Bind the buffer to the appropriate buffer slot.
	glBindBuffer( m_iOpenGlBindFlags, m_iHandle );
  CheckForGLESError();
}

bool
GLES2HardwareBufferImpl::Load()
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
    NE_ASSERT( false, "Opengl ES 2.0 doesn't support uniform buffers!" )();
		//m_iOpenGlBindFlags = GL_UNIFORM_BUFFER;
	}

	glGenBuffers( 1, &m_iHandle );
	CheckForGLESError();

	WriteData( 0, m_SizeInBytes, m_SysMem, false );

	return true;
}

void
GLES2HardwareBufferImpl::WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer )
{
	Bind();

  const void* pBuffer = static_cast<const unsigned char*>(pSource) + offset;

	// Load the data into the buffer.
	glBufferData( m_iOpenGlBindFlags, (GLsizeiptr)length, pBuffer, m_iOpenGlUsage ); //< this could be using glBufferSubData for existing buffers?
  CheckForGLESError();
}

void*
GLES2HardwareBufferImpl::Lock( std::size_t offset, size_t length, LockOptions options )
{
  void* retptr = nullptr;

//#if WIN32
//  Bind();
//
//  GLenum access = 0;
//  if( (options & HBL_DISCARD) == HBL_DISCARD )
//	{
//		// Discard the buffer
//		glBufferData( GL_ARRAY_BUFFER, m_SizeInBytes, NULL, GetGLUsage(m_Usage) );
//    CheckForGLESError();
//	}
//
//	if( (m_Usage & HBU_WRITE_ONLY) == HBU_WRITE_ONLY )
//	{
//   	access = GL_WRITE_ONLY;
//  }
//	else if( options == HBL_READ_ONLY )
//	{
//   	access = GL_READ_ONLY;
//  }
//	else
//	{
//   	access = GL_READ_WRITE;
//  }
//
//	void* pBuffer = glMapBuffer( GL_ARRAY_BUFFER, access );
//  CheckForGLESError();
//
//	NE_ASSERT( pBuffer != nullptr, "Vertex Buffer: Out of memory" )();
//	
//	// return offsetted data
//	retptr = static_cast<void*>(
//		static_cast<unsigned char*>(pBuffer) + offset);
//#endif

  //@todo - throw an exception on iOS due to unsupported function?

  return retptr;
}

void
GLES2HardwareBufferImpl::Unlock()
{
	//Bind();

  ////let buffer know that we are done modifying data.
	//glUnmapBuffer( m_iOpenGlBindFlags );

  //CheckForGLESError();
}

GLenum 
GLES2HardwareBufferImpl::GetGLUsage( const Flags<HardwareBufferUsage>& usage )
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
    return GL_STATIC_DRAW;
  }
  
  return GL_DYNAMIC_DRAW;
}