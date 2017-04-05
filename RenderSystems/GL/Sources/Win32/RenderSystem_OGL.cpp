 
#include "Includes/RenderSystem_OGL.h"

#include <Nebulae/Common/Window/Win32/Win32Window.h>

#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

#include <Includes/HardwareShaderImpl_OGL.h>
#include <Includes/InputLayoutImpl_OGL.h>
#include <Includes/ProgramObject.h>
#include <Includes/HardwareBufferImpl_OGL.h>
#include <Includes/SamplerImpl_OGL.h>
#include <Includes/TextureImpl_OGL.h>


PFNGLUNIFORM1FPROC        glUniform1f        = NULL;
PFNGLUNIFORM2FPROC        glUniform2f        = NULL;
PFNGLUNIFORM3FPROC        glUniform3f        = NULL;
PFNGLUNIFORM4FPROC        glUniform4f        = NULL;
PFNGLUNIFORM1IPROC        glUniform1i        = NULL;
PFNGLUNIFORM2IPROC        glUniform2i        = NULL;
PFNGLUNIFORM3IPROC        glUniform3i        = NULL;
PFNGLUNIFORM4IPROC        glUniform4i        = NULL;
PFNGLUNIFORM1FVPROC       glUniform1fv       = NULL;
PFNGLUNIFORM2FVPROC       glUniform2fv       = NULL;
PFNGLUNIFORM3FVPROC       glUniform3fv       = NULL;
PFNGLUNIFORM4FVPROC       glUniform4fv       = NULL;
PFNGLUNIFORM1IVPROC       glUniform1iv       = NULL;
PFNGLUNIFORM2IVPROC       glUniform2iv       = NULL;
PFNGLUNIFORM3IVPROC       glUniform3iv       = NULL;
PFNGLUNIFORM4IVPROC       glUniform4iv       = NULL;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;



#define BUFFER_OFFSET(x)  ((char*)NULL + (x))


using namespace Nebulae;


bool CheckForGLError()
{
  GLuint error = glGetError();

  NE_ASSERT( error == GL_NO_ERROR, "OpenGL error 0x%x (%u) detected", error, error )();
  
  return error == GL_NO_ERROR;
}

RenderSystem_OGL::RenderSystem_OGL( FileSystemPtr fileSystem, WindowPtr window )
	: RenderSystem( fileSystem, window )
	, m_name( "gl3" )
  , m_hDC( NULL )
	, m_hRC( NULL )
	, m_OperationMode( 0 )
	, m_boundProgram( NULL )
{}


RenderSystem_OGL::~RenderSystem_OGL()
{}


const std::string&
RenderSystem_OGL::GetName() const
{ return m_name; }


bool
RenderSystem_OGL::Initiate()
{
	BYTE bits = 32;
	
	// pfd Tells Windows How We Want Things To Be
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),          // Size Of This Pixel Format Descriptor
		1,																			// Version Number
		PFD_DRAW_TO_WINDOW |                    // Format Must Support Window
		PFD_SUPPORT_OPENGL |                    // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		bits,																		// Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,																			// No Alpha Buffer
		0,																			// Shift Bit Ignored
		0,																			// No Accumulation Buffer
		0, 0, 0, 0,															// Accumulation Bits Ignored
		16,																			// 16Bit Z-Buffer (Depth Buffer)
		0,																			// No Stencil Buffer
		0,																			// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																			// Reserved
		0, 0, 0																	// Layer Masks Ignored
	};

	// Did We Get A Device Context?
	m_hDC = std::static_pointer_cast<Win32Window>(m_window)->GetDC();
	if( m_hDC == NULL ) {
		//
		//@todo Reset The Display
		//
		MessageBox( NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}
	
	// Did Windows Find A Matching Pixel Format?
	int iPixelFormat = ChoosePixelFormat( m_hDC, &pfd );
	if( iPixelFormat == 0 ) {
		//
		//@todo Reset The Display
		//
		MessageBox( NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	// Are We Able To Set The Pixel Format?
	if( !SetPixelFormat( m_hDC, iPixelFormat, &pfd ) ) {
		//
		//@todo Reset The Display
		//
		MessageBox( NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}
	
	// Are We Able To Get A Rendering Context?
	m_hRC = wglCreateContext( m_hDC );
	if( m_hRC == NULL ) {
		//
		//@todo Reset The Display
		//
		MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

	// Try To Activate The Rendering Context
	if( !wglMakeCurrent( m_hDC, m_hRC ) ) {
		//
		//@todo Reset The Display
		//
		MessageBox( NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	// Setup some basic OpenGl state calls
	SetClearColour( 0.0f, 0.0f, 0.0f, 0.0f ); 
	glClearDepth( 1.0f );
  glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS ); 
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); 
  glEnable( GL_CULL_FACE );


	int iHeight = m_window->GetHeight(), iWidth = m_window->GetWidth();
	if ( iHeight == 0 ) {
    iHeight = 1;
  }
 
  glViewport( 0, 0, iWidth, iHeight ); 

	// Initialize the required opengl functions.
	HardwareBufferImpl_OGL::initiateFunctions();
	HardwareShaderImpl_OGL::initiateFunctions();
	InputLayoutImpl_OGL::initiateFunctions();
	ProgramObject::initiateFunctions();
	
	// Initialize the functions required by this class.
	glUniform1f          = (PFNGLUNIFORM1FPROC)wglGetProcAddress( "glUniform1f" );
	glUniform2f          = (PFNGLUNIFORM2FPROC)wglGetProcAddress( "glUniform2f" );
	glUniform3f          = (PFNGLUNIFORM3FPROC)wglGetProcAddress( "glUniform3f" );
	glUniform4f          = (PFNGLUNIFORM4FPROC)wglGetProcAddress( "glUniform4f" );
	glUniform1i          = (PFNGLUNIFORM1IPROC)wglGetProcAddress( "glUniform1i" );
	glUniform2i          = (PFNGLUNIFORM2IPROC)wglGetProcAddress( "glUniform2i" );
	glUniform3i          = (PFNGLUNIFORM3IPROC)wglGetProcAddress( "glUniform3i" );
	glUniform4i          = (PFNGLUNIFORM4IPROC)wglGetProcAddress( "glUniform4i" );
	glUniform1fv         = (PFNGLUNIFORM1FVPROC)wglGetProcAddress( "glUniform1fv" );
	glUniform2fv         = (PFNGLUNIFORM2FVPROC)wglGetProcAddress( "glUniform2fv" );
	glUniform3fv         = (PFNGLUNIFORM3FVPROC)wglGetProcAddress( "glUniform3fv" );
	glUniform4fv         = (PFNGLUNIFORM4FVPROC)wglGetProcAddress( "glUniform4fv" );
	glUniform1iv         = (PFNGLUNIFORM1IVPROC)wglGetProcAddress( "glUniform1iv" );
	glUniform2iv         = (PFNGLUNIFORM2IVPROC)wglGetProcAddress( "glUniform2iv" );
	glUniform3iv         = (PFNGLUNIFORM3IVPROC)wglGetProcAddress( "glUniform3iv" );
	glUniform4iv         = (PFNGLUNIFORM4IVPROC)wglGetProcAddress( "glUniform4iv" );
	glUniformMatrix2fv   = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress( "glUniformMatrix2fv" );
	glUniformMatrix3fv   = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress( "glUniformMatrix3fv" );
	glUniformMatrix4fv   = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress( "glUniformMatrix4fv" );


	return RenderSystem::Initiate();
}


void 
RenderSystem_OGL::Clear()
{ glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); }


void
RenderSystem_OGL::SetClearColour( float r, float g, float b, float a )
{ glClearColor(r,g,b,a); }


void
RenderSystem_OGL::SwapBuffers()
{
	//@todo [jared.watt] Implement vsync functionality
	::SwapBuffers( m_hDC );
}


void 
RenderSystem_OGL::SetBlendingState( bool enable )
{
  if( enable ) {
    glEnable( GL_BLEND );
  } else {
    glDisable( GL_BLEND );
  }

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


void
RenderSystem_OGL::SetDepthTest( bool enable )
{
  if( enable ) {
    glEnable( GL_DEPTH_TEST );
  }
  else {
    glDisable( GL_DEPTH_TEST );
  }

  glDepthFunc( GL_LESS );
}


HardwareBufferImpl* 
RenderSystem_OGL::CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
{ return new HardwareBufferImpl_OGL( usage, sizeInBytes, bindFlags, sysMem ); }


HardwareShaderImpl*
RenderSystem_OGL::CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType )
{ return new HardwareShaderImpl_OGL( strFileName, eType ); }


InputLayoutImpl*
RenderSystem_OGL::CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* vertexShader )
{ return new InputLayoutImpl_OGL( pVertexDecl, vertexShader ); }


Sampler::Impl*
RenderSystem_OGL::CreateSamplerImpl()
{ return new SamplerImpl_OGL(); }


TextureImpl*
RenderSystem_OGL::CreateTextureImpl( const std::string& strName )
{ return new TextureImpl_OGL( strName ); }


void
RenderSystem_OGL::SetVertexBuffers( int iSlot, HardwareBuffer* pBufferImpl, size_t iStride, size_t iOffset )
{
	// @note [jared.watt] Should check that this is actually a vertex buffer.
  static_cast< HardwareBufferImpl_OGL* >( pBufferImpl->GetImpl() )->Bind();
}


void
RenderSystem_OGL::SetInputLayout( InputLayout* inputLayout )
{ 
  NE_ASSERT( m_boundProgram != NULL, "Cannot set InputLayout before setting shaders" )();
	static_cast< InputLayoutImpl_OGL* >( inputLayout->GetImpl() )->Bind( m_boundProgram );
}


void
RenderSystem_OGL::SetIndexBuffer( HardwareBuffer* pBufferImpl, size_t iOffset )
{
	// @note [jared.watt] Should check that this is actually a vertex buffer.
	static_cast< HardwareBufferImpl_OGL* >( pBufferImpl->GetImpl() )->Bind();
}


void 
RenderSystem_OGL::SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader )
{
  NE_ASSERT( vertexShader->GetImpl()->GetType() == VERTEX_SHADER, "Shader in VertexShader slot is not a VertexShader" )();
  NE_ASSERT( fragmentShader->GetImpl()->GetType() == PIXEL_SHADER, "Shader in PixelShader slot is not a PixelShader" )();

  m_vertexShader   = vertexShader;
  m_fragmentShader = fragmentShader;
  m_boundProgram   = NULL;

  //@todo.  Figure out how we are going to handle the "binding" of the program object.
  ProgramObjectList::const_iterator programItr = m_programObjects.begin();
  for( ; programItr != m_programObjects.end(); ++programItr ) {
    if( (*programItr)->GetVertexShader() == vertexShader && (*programItr)->GetFragmentShader() == fragmentShader ) {
      m_boundProgram = (*programItr);
      break;
    }
  }

  // Check if found and if not create a new ProgramObject
  if( NULL == m_boundProgram )
  {
    m_boundProgram = new ProgramObject( m_vertexShader, m_fragmentShader );
    m_programObjects.push_back( m_boundProgram );
    m_boundProgram->Load();
  }

  m_boundProgram->UseProgram();
}

void
RenderSystem_OGL::SetOperationType( OperationType eType )
{
	// @todo [jared.watt] Implement other topologies
  switch( eType )
  {
    case OT_POINTS:       m_OperationMode = GL_POINTS; break;
	  case OT_LINES:        m_OperationMode = GL_LINES; break;
	  case OT_LINELIST:     m_OperationMode = GL_LINE_STRIP; break;
	  case OT_TRIANGLES:    m_OperationMode = GL_TRIANGLES; break;
	  case OT_TRIANGLELIST: m_OperationMode = GL_TRIANGLE_STRIP; break; 
	  case OT_TRIANGLEFAN:  m_OperationMode = GL_TRIANGLE_FAN; break; 
    
    case OT_UNKNOWN:
    default:
      NE_BREAKPOINT; break;
  }
}
		

void
RenderSystem_OGL::Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation )
{ glDrawArrays( m_OperationMode, (int)iStartVertexLocation, (int)iVertexCount ); }


void
RenderSystem_OGL::DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation )
{ glDrawElements( m_OperationMode, (int)iIndexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(iStartIndexLocation) ); }


void
RenderSystem_OGL::SetBufferBinding( uint32 iTarget, uint32 iIndex, HardwareBuffer* pImpl )
{
	//@todo Not yet implemented.
	NE_BREAKPOINT;
}


UniformDefinition 
RenderSystem_OGL::GetUniformByName( const char* name ) const
{
  NE_ASSERT( m_boundProgram != NULL, "Attempting to find a uniform before any shaders are bound" )( name );
  if( NULL == m_boundProgram ) {
    return UniformDefinition();
  }

  return m_boundProgram->FindUniformByName( name );
}


void
RenderSystem_OGL::SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* pImpl )
{}


void
RenderSystem_OGL::SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* pImpl )
{}


void
RenderSystem_OGL::SetUniformBinding( UniformDefinition& definition, void* value )
{
  GLint location = (GLint)definition.logicalIndex;  //< this is bad... since this value shouldnt even be -1...
	if( location == -1 ) {
    NE_ASSERT( location != -1, "Invalid uniform found." )( location );
    return;
  }

	switch( definition.type ) {
  case UT_FLOAT1: {
			GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniform1fv( location, 1, buffer );
		} break;

	case UT_FLOAT2: {
			GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniform2fv( location, 1, buffer );
		} break;

  case UT_FLOAT3: {
			GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniform3fv( location, 1, buffer );
		} break;

  case UT_FLOAT4: {
			GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniform4fv( location, 1, buffer );
		} break;

  case UT_MATRIX_3X3: {
      GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniformMatrix3fv( location, 1, GL_FALSE, buffer );
    } break;

	case UT_MATRIX_4X4: {
			GLfloat* buffer = static_cast<GLfloat* >( value );
			glUniformMatrix4fv( location, 1, GL_FALSE, buffer );
		} break;

  case UT_SAMPLER2D: {
      int32            identifier = *static_cast<int32* >( value );
      Texture*         texture    = FindTextureByIdentifier( identifier );
      TextureImpl_OGL* impl       = static_cast<TextureImpl_OGL*>( texture->GetImpl() );

      GLuint handle = impl->GetHandle();
      glBindTexture( GL_TEXTURE_2D, handle );
      glUniform1i( location, 0 );
    } break;
  }
}