
#include "GLES2RenderSystem.h"
#include "TouchesDelegate.h"

#import  <QuartzCore/QuartzCore.h>

#include <Nebulae/Common/Window/iOS/Window_iOS.h>

#include <Nebulae/Alpha/GLES2/Shared/GLES2HardwareBufferImpl.h>
#include <Nebulae/Alpha/GLES2/Shared/GLES2HardwareShaderImpl.h>
#include <Nebulae/Alpha/GLES2/Shared/GLES2InputLayoutImpl.h>
#include <Nebulae/Alpha/GLES2/Shared/GLES2SamplerImpl.h>
#include <Nebulae/Alpha/GLES2/Shared/GLES2TextureImpl.h>
#include <Nebulae/Alpha/GLES2/Shared/ProgramObject.h>


using namespace Nebulae;


//Constructor
GLES2RenderSystem::GLES2RenderSystem( FileSystemArchive& fileSystem, Window* window )
  : RenderSystem( fileSystem, window ),
    m_defaultframebuffer( 0 ),
    m_colorrenderbuffer( 0 ),
    m_depthrenderbuffer( 0 ),  
    m_boundProgram( NULL ),
    m_vertexShader( NULL ),
    m_fragmentShader( NULL ),
#ifdef __OBJC__
    m_pContext( nil ),
    m_pViewController( nil )
#endif
{

}

//Destructor
GLES2RenderSystem::~GLES2RenderSystem()
{
	DestroyFramebuffer();
}


const std::string 
GLES2RenderSystem::GetName() const
{ return "gles"; }


bool
GLES2RenderSystem::Initiate()
{
  m_pViewController = [[EAGLViewController alloc] init];
  
  TouchesDelegate* touchesDelegate = new TouchesDelegate( m_window, m_pViewController.view.contentScaleFactor );
  [m_pViewController setTouchesDelegate:touchesDelegate];

  UIWindow* window = static_cast<Window_iOS*>(m_window)->GetHandle();
  [window setRootViewController:m_pViewController];

  
	[m_pViewController.view setUserInteractionEnabled:YES];
  [m_pViewController.view setMultipleTouchEnabled:YES];
  
  m_pContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  if( !m_pContext || ![EAGLContext setCurrentContext:m_pContext] ) {
    assert( m_pContext != nil && "GLES2RenderSystem: Failed to create OpenGL ES context" );
  }

  // Create the frame buffer for rendering
  CreateFramebuffer();

  SetClearColour( 0.0f, 0.0f, 0.0f, 0.0f );
  //glClearDepth( 1.0f );
  //glShadeModel( GL_SMOOTH );
  //glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS ); 
  //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glEnable( GL_CULL_FACE );

  int32 width   = m_window->GetWidth();
  int32 height  = m_window->GetHeight();
  glViewport( 0, 0, width, height );
  
  return RenderSystem::Initiate();
}


void
GLES2RenderSystem::CreateFramebuffer()
{
	assert( m_defaultframebuffer == 0 );
	
	// Create default framebuffer object and bind it.
	glGenFramebuffers( 1, &m_defaultframebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, m_defaultframebuffer );
	
	// Create color render buffer
	glGenRenderbuffers( 1, &m_colorrenderbuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, m_colorrenderbuffer );
	
	// Get the storage from iOS so it can be displayed in the view.
	[m_pContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)m_pViewController.view.layer];
	
  // Get the frames width and height
	int framebufferwidth, framebufferheight;
	glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferwidth );
	glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferheight );
	
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorrenderbuffer );
	
	// Create a depth renderbuffer
	glGenRenderbuffers( 1, &m_depthrenderbuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, m_depthrenderbuffer );
	// Create the storeage for the buffer, optimized for depth values, same size as the colorrenderbuffer
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferwidth, framebufferheight );
	// Attach the depth buffer for our framebuffer
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthrenderbuffer );
	
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {
		NSLog( @"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER) );
	}
}
			  

void
GLES2RenderSystem::DestroyFramebuffer()
{
	// We need a valid and current context to access any OpenGL methods
	if( m_pContext ) {
		[EAGLContext setCurrentContext:m_pContext];
		
		if( m_defaultframebuffer ) {
			glDeleteFramebuffers( 1, &m_defaultframebuffer );
			m_defaultframebuffer = 0;
		}
	
		if( m_colorrenderbuffer ) {
			glDeleteRenderbuffers( 1, &m_colorrenderbuffer );
			m_colorrenderbuffer = 0;
		}
				
		if( m_depthrenderbuffer ) {
			glDeleteRenderbuffers( 1, &m_depthrenderbuffer );
			m_depthrenderbuffer = 0;
		}
	}
}


void
GLES2RenderSystem::Clear()
{
  //make it the current context for rendering
  [EAGLContext setCurrentContext:m_pContext];
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultframebuffer);
	
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void
GLES2RenderSystem::SetClearColour( Real r, Real g, Real b, Real a )
{	
  //make it the current context for rendering
  [EAGLContext setCurrentContext:m_pContext];
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultframebuffer);
  	
  glClearColor(r,g,b,a);
}

void 
GLES2RenderSystem::SetBlendingState( bool enable )
{
  if( enable ) {
    glEnable( GL_BLEND );
  } else {
    glDisable( GL_BLEND );
  }

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


void
GLES2RenderSystem::SetDepthTest( bool enable )
{
  if( enable ) {
    glEnable( GL_DEPTH_TEST );
  } else {
    glDisable( GL_DEPTH_TEST );
  }
  
  glDepthFunc( GL_LESS );
}


void 
GLES2RenderSystem::SwapBuffers( bool bWaitForVsync )
{
  //make it the current context for rendering
  [EAGLContext setCurrentContext:m_pContext];
  glBindFramebuffer( GL_FRAMEBUFFER, m_defaultframebuffer );
	
  // Finally, get the color buffer we rendered to, and pass it to iOS so it can display.
  glBindRenderbuffer( GL_RENDERBUFFER, m_colorrenderbuffer );
  [m_pContext presentRenderbuffer:GL_RENDERBUFFER];
}


HardwareBufferImpl*
GLES2RenderSystem::CreateBufferImpl( HardwareBufferUsage usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
{ return new GLES2HardwareBufferImpl( usage, sizeInBytes, bindFlags, sysMem ); }


HardwareShaderImpl*
GLES2RenderSystem::CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType )
{ return new GLES2HardwareShaderImpl( strFileName, eType ); }


InputLayout::Impl*
GLES2RenderSystem::CreateInputLayoutImpl( VertexDeceleration* pVertexDecl )
{ return new GLES2InputLayoutImpl( pVertexDecl ); }


Sampler::Impl*
GLES2RenderSystem::CreateSamplerImpl()
{	return new GLES2SamplerImpl(); }


TextureImpl*
GLES2RenderSystem::CreateTextureImpl( const std::string& strName )
{ return new GLES2TextureImpl( strName ); }


void
GLES2RenderSystem::SetVertexBuffers( int iSlot, HardwareBuffer* pBuffer, std::size_t iStride, std::size_t iOffset )
{
  //@todo I would like to assert that this is actally a vertex buffer.
  static_cast< GLES2HardwareBufferImpl* >( pBuffer->GetImpl() )->Bind();
}


void
GLES2RenderSystem::SetInputLayout( InputLayout::Impl* pInputLayoutImpl )
{ static_cast< GLES2InputLayoutImpl* >( pInputLayoutImpl )->Bind( m_boundProgram ); }


void
GLES2RenderSystem::SetIndexBuffer( HardwareBuffer* pBuffer, uint32 iOffset )
{ static_cast< GLES2HardwareBufferImpl* >( pBuffer->GetImpl() )->Bind(); }


void
GLES2RenderSystem::SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader )
{
  assert( vertexShader->GetImpl()->GetType() == VERTEX_SHADER );
  assert( fragmentShader->GetImpl()->GetType() == PIXEL_SHADER );

  m_vertexShader   = vertexShader;
  m_fragmentShader = fragmentShader;
  m_boundProgram   = NULL;

  //@todo.  Figure out how we are going to handle the "binding" of the program object.
  ProgramObjectList::const_iterator programItr = m_programObjects.begin();
  for( ; programItr != m_programObjects.end(); ++programItr ) {
    if( (*programItr)->GetVertexShader() == vertexShader && (*programItr)->GetFragmentShader() == fragmentShader ) {
      m_boundProgram = (*programItr);
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
GLES2RenderSystem::SetOperationType( OperationType eType )
{
  // @todo [jared.watt] Implement other topologies
  switch( eType )
  {
    case OT_POINTS:       m_operationMode = GL_POINTS; break;
    case OT_LINES:        m_operationMode = GL_LINES; break;
    case OT_LINELIST:     m_operationMode = GL_LINE_STRIP; break;
    case OT_TRIANGLES:    m_operationMode = GL_TRIANGLES; break;
    case OT_TRIANGLELIST: m_operationMode = GL_TRIANGLE_STRIP; break;
    case OT_TRIANGLEFAN:  m_operationMode = GL_TRIANGLE_FAN; break;
    
    case OT_UNKNOWN:
    default:
      assert( 0 ); break;
  }
}


void
GLES2RenderSystem::Draw( int iVertexCount, int iStartVertexLocation )
{
	glDrawArrays( m_operationMode, iStartVertexLocation, iVertexCount );
}


void
GLES2RenderSystem::DrawIndexed( int iIndexCount, int iStartIndexLocation, int iBaseVertexLocation )
{
	glDrawElements( m_operationMode, iIndexCount, GL_UNSIGNED_SHORT, (const GLvoid*)iStartIndexLocation);
	//glDrawRangeElements( m_OperationMode, iStartIndexLocation, iStartIndexLocation + iIndexCount, iIndexCount, GL_UNSIGNED_SHORT, NULL );
}


UniformDefinition 
GLES2RenderSystem::GetUniformByName( const char* name ) const
{
  assert( m_boundProgram );
  if( NULL == m_boundProgram ) {
    return UniformDefinition();
  }

  return m_boundProgram->FindUniformByName( name );
}


void
GLES2RenderSystem::SetBufferBinding( uint32 iTarget, uint32 iIndex, HardwareBuffer* pImpl )
{
	//
	//@todo Cant actually use Uniform buffers in gles. :(
	//
	assert( 0 );
}


void
GLES2RenderSystem::SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler::Impl* pImpl )
{

}


void
GLES2RenderSystem::SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* pImpl )
{

}


void
GLES2RenderSystem::SetUniformBinding( UniformDefinition& definition, void* value )
{
	GLint location = definition.logicalIndex; 
  if( location == -1 ) {
    assert( location != -1 && "Invalid uniform found." );
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
        int32             identifier = *static_cast<int32*>( value );
        Texture*          texture    = FindTextureByIdentifier( identifier );
        GLES2TextureImpl* impl       = static_cast<GLES2TextureImpl*>( texture->GetImpl() );

        glBindTexture( GL_TEXTURE_2D, impl->GetHandle() );
        glUniform1i( location, 0 ); 
      } break;
  }
}

RenderSystem* 
Nebulae::CreateRenderSystem( FileSystemArchive& fileSystem, Window* window )
{
  return new GLES2RenderSystem( fileSystem, window );
}

bool
CheckForGLESError()
{
  GLuint error = glGetError();
  if( error != GL_NO_ERROR )
  {
    assert( !"OpenGL error 0x%x (%u) detected" );
    return true;
  }
  return false;
}