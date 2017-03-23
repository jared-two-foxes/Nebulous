
#include "RenderSystem_GLES2_Win32.h"

#include <Nebulae/Common/Window/Win32/Win32Window.h>

#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

#include <RenderSystems/GLES2/Shared/GLES2HardwareShaderImpl.h>
#include <RenderSystems/GLES2/Shared/GLES2InputLayoutImpl.h>
#include <RenderSystems/GLES2/Shared/ProgramObject.h>
#include <RenderSystems/GLES2/Shared/GLES2HardwareBufferImpl.h>
#include <RenderSystems/GLES2/Shared/GLES2RenderTextureImpl.h>
#include <RenderSystems/GLES2/Shared/GLES2SamplerImpl.h>
#include <RenderSystems/GLES2/Shared/GLES2TextureImpl.h>


#define BUFFER_OFFSET(x)  ((char*)NULL + (x))


//class DepthFuncSetter 
//{ 
//private: 
//  GLint m_oldSetting; 
//
//  public: 
//    DepthFuncSetter( GLenum f ) 
//    { 
//      glGetIntegerv( GL_DEPTH_FUNC, &m_oldSetting ); 
//      glDepthFunc( f ); 
//    } 
//  
//    ~DepthFuncSetter() 
//    { 
//      glDepthFunc( m_oldSetting ); 
//    }
//};


using namespace Nebulae;


bool CheckForGLESError()
{
  GLuint error = glGetError();

  NE_ASSERT( error == GL_NO_ERROR, "OpenGL error 0x%x (%u) detected", error, error )();
  
  return error == GL_NO_ERROR;
}

RenderSystem_GLES2_Win32::RenderSystem_GLES2_Win32( FileSystemPtr fileSystem, WindowPtr window )
: RenderSystem( fileSystem, window ), 
	m_name( "gles" ),
  m_OperationMode( 0 ), 
  m_boundProgram( nullptr ),
  m_vertexShader( nullptr ),
  m_fragmentShader( nullptr ),
  m_inputLayout( nullptr ),
  m_renderTexture( nullptr )
{}

RenderSystem_GLES2_Win32::~RenderSystem_GLES2_Win32()
{}

const std::string&
RenderSystem_GLES2_Win32::GetName() const
{ return m_name; }

bool
RenderSystem_GLES2_Win32::Initiate()
{
  // Store the windows DeviceContext
  std::shared_ptr<Win32Window > window = std::dynamic_pointer_cast<Win32Window >(m_window);
  HWND hwnd = window->GetWnd();
  HDC  hDC  = window->GetDC();
  if( hDC == NULL ) {
    MessageBox( NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;
  }
  
  m_display = eglGetDisplay( hDC );

  EGLBoolean didInitialize = eglInitialize( m_display, NULL, NULL );

  EGLint attributes[] = {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT /*| EGL_OPENGL_ES3_BIT_KHR*/, // Request OpenGL ES 2.0 configs
    //EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_DEPTH_SIZE, 16,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_NONE
  };
  EGLConfig  configs[1];
  EGLint     num_configs;
  EGLBoolean result = eglChooseConfig( m_display, attributes, configs, 1, &num_configs );

  m_surface = eglCreateWindowSurface( m_display, configs[0], (NativeWindowType)hwnd, 0 ); 

  EGLint context_attributes[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2, // Select an OpenGL ES 2.0 context
    EGL_NONE
  };
  EGLContext context = eglCreateContext( m_display, configs[0], EGL_NO_CONTEXT, context_attributes );
  if( context == NULL ) {
    MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return FALSE;
  }

  // Try To Activate The Rendering Context
  if( !eglMakeCurrent( m_display, m_surface, m_surface, context ) ) {
    MessageBox( NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;
  }

  SetClearColour( 0.0f, 0.0f, 0.0f, 0.0f ); 
  
  glDepthFunc( GL_LESS ); 
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glDepthFunc( GL_LESS );

  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );

  int32 iHeight = m_window->GetHeight();
  int32 iWidth  = m_window->GetWidth();
  if( iHeight == 0 ) 
  {
    iHeight = 1;
  }
 
  glViewport( 0, 0, iWidth, iHeight ); 

  return RenderSystem::Initiate();
}

void 
RenderSystem_GLES2_Win32::Clear()
{ 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
}

void
RenderSystem_GLES2_Win32::SetClearColour( float r, float g, float b, float a )
{ 
  glClearColor(r,g,b,a); 
}

void
RenderSystem_GLES2_Win32::SwapBuffers()
{
  eglSwapBuffers( m_display, m_surface ); 
}

void 
RenderSystem_GLES2_Win32::SetBlendingState( bool enable )
{
  if( enable ) {
    glEnable( GL_BLEND );
  } else {
    glDisable( GL_BLEND );
  }
}

void
RenderSystem_GLES2_Win32::SetDepthTest( bool enable )
{
  if( enable ) {
    glEnable( GL_DEPTH_TEST );
  } else {
    glDisable( GL_DEPTH_TEST );
  }
}

void 
RenderSystem_GLES2_Win32::BeginScissorClipping( Point ul, Point lr )
{
  if( m_scissor_clipping_rects.empty() )
  {
    glEnable( GL_SCISSOR_TEST );

    //if( g_stencil_bit ) //< This appears to be a test to query whether a stencil test is currently enabled.
    //{
    //  glDisable( GL_STENCIL_TEST );
    //}
  } 
  else
  {
    const Rect& r = m_scissor_clipping_rects.back();
    ul.x = std::max( r.GetLeft(), std::min(ul.x, r.GetRight()) );
    ul.y = std::max( r.GetTop(),  std::min(ul.y, r.GetBottom()) );
    lr.x = std::max( r.GetLeft(), std::min(lr.x, r.GetRight()) );
    lr.y = std::max( r.GetTop(),  std::min(lr.y, r.GetBottom()) );
  }

  glScissor( Value(ul.x), Value(m_window->GetHeight() - lr.y),
             Value(lr.x - ul.x), Value(lr.y - ul.y) );

  m_scissor_clipping_rects.push_back( Rect(ul, lr) );
}

void 
RenderSystem_GLES2_Win32::EndScissorClipping()
{
  NE_ASSERT( !m_scissor_clipping_rects.empty(), "" )();

  m_scissor_clipping_rects.pop_back();
  if( m_scissor_clipping_rects.empty() )
  {
    glDisable( GL_SCISSOR_TEST ); //< this was not here as the PopAttrib would handle this so that if it was ever set outside of the render pipeline it would be preserved.  Should be fine to ignore now.

    //if( g_stencil_bit )
    //{
    //  glEnable( GL_STENCIL_TEST );
    //}
  } 
  else 
  {
    const Rect& r = m_scissor_clipping_rects.back();
    glScissor( Value(r.GetLeft()), Value(m_window->GetHeight() - r.GetBottom()),
               Value(r.GetWidth()), Value(r.GetHeight()) );
  }
}

void 
RenderSystem_GLES2_Win32::BeginStencilClipping( Point inner_ul, Point inner_lr, Point outer_ul, Point outer_lr )
{
//  if (!g_stencil_bit) {
//      glPushAttrib(GL_STENCIL_BUFFER_BIT);
//      glClearStencil(0);
//      glClear(GL_STENCIL_BUFFER_BIT);
//      glEnable(GL_STENCIL_TEST);
//      if (!g_scissor_clipping_rects.empty())
//          glDisable(GL_SCISSOR_TEST);
//  }
//
//  glGetBooleanv(GL_COLOR_WRITEMASK, g_prev_color_writemask);
//  glGetBooleanv(GL_DEPTH_WRITEMASK, &g_prev_depth_writemask);
//
//  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//  glDepthMask(GL_FALSE);
//
//  GLuint mask = 1u << g_stencil_bit;
//
//  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
//  glEnableClientState(GL_VERTEX_ARRAY);
//
//  glStencilFunc(GL_ALWAYS, mask, mask);
//  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
//  GLint outer_vertices[] = {
//      Value(outer_ul.x), Value(outer_ul.y),
//      Value(outer_ul.x), Value(outer_lr.y),
//      Value(outer_lr.x), Value(outer_lr.y),
//      Value(outer_lr.x), Value(outer_ul.y)
//  };
//  glVertexPointer(2, GL_INT, 0, outer_vertices);
//  glDrawArrays(GL_QUADS, 0, 4);
//
//  glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
//  GLint inner_vertices[] = {
//      Value(inner_ul.x), Value(inner_ul.y),
//      Value(inner_ul.x), Value(inner_lr.y),
//      Value(inner_lr.x), Value(inner_lr.y),
//      Value(inner_lr.x), Value(inner_ul.y)
//  };
//  glVertexPointer(2, GL_INT, 0, inner_vertices);
//  glDrawArrays(GL_QUADS, 0, 4);
//
//  glColorMask(g_prev_color_writemask[0],
//              g_prev_color_writemask[1],
//              g_prev_color_writemask[2],
//              g_prev_color_writemask[3]);
//  glDepthMask(g_prev_depth_writemask);
//
//  glStencilFunc(GL_EQUAL, mask, mask);
//  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//  ++g_stencil_bit;
//
//  glPopClientAttrib();
}

void 
RenderSystem_GLES2_Win32::EndStencilClipping()
{
//  assert(g_stencil_bit);
//  --g_stencil_bit;
//  if (!g_stencil_bit) {
//      glPopAttrib();
//      if (!g_scissor_clipping_rects.empty())
//          glEnable(GL_SCISSOR_TEST);
//  }
}

HardwareBufferImpl* 
RenderSystem_GLES2_Win32::CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
{ return new GLES2HardwareBufferImpl( usage, sizeInBytes, bindFlags, sysMem ); }

HardwareShaderImpl*
RenderSystem_GLES2_Win32::CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType )
{ return new GLES2HardwareShaderImpl( strFileName, eType ); }

InputLayoutImpl*
RenderSystem_GLES2_Win32::CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader )
{ return new GLES2InputLayoutImpl( pVertexDecl, pVertexShader ); }

RenderTextureImpl*  
RenderSystem_GLES2_Win32::CreateRenderTextureImpl( int32 width, int height )
{ return new GLES2RenderTextureImpl( width, height ); }

Sampler::Impl*
RenderSystem_GLES2_Win32::CreateSamplerImpl()
{ return new GLES2SamplerImpl(); }

TextureImpl*
RenderSystem_GLES2_Win32::CreateTextureImpl( const std::string& strName )
{ return new GLES2TextureImpl( strName ); }

void
RenderSystem_GLES2_Win32::SetVertexBuffers( int iSlot, HardwareBuffer* pBufferImpl, size_t iStride, size_t iOffset )
{
  // @note [jared.watt] Should check that this is actually a vertex buffer.
  static_cast< GLES2HardwareBufferImpl* >( pBufferImpl->GetImpl() )->Bind();
}

void
RenderSystem_GLES2_Win32::SetInputLayout( InputLayout* pInputLayout )
{ 
  if( m_inputLayout != pInputLayout )
  {
    NE_ASSERT( m_boundProgram, "A program object must be bound before an InputLayout can be set!" )();
    
    if( m_inputLayout != nullptr )
    {
      NE_ASSERT( m_inputLayout->GetImpl(), "Found an input layout that doesnt have a valid implementation" )();
      static_cast<GLES2InputLayoutImpl* >(m_inputLayout->GetImpl())->Unbind();
    }

    m_inputLayout = pInputLayout;

    if( m_inputLayout != nullptr )
    {
      NE_ASSERT( m_inputLayout->GetImpl(), "Found an input layout that doesnt have a valid implementation" )();
      static_cast<GLES2InputLayoutImpl* >(m_inputLayout->GetImpl())->Bind( m_boundProgram );
    }
  }
}

void
RenderSystem_GLES2_Win32::SetIndexBuffer( HardwareBuffer* pBufferImpl, size_t iOffset )
{
  // @note [jared.watt] Should check that this is actually a vertex buffer.
  static_cast< GLES2HardwareBufferImpl* >( pBufferImpl->GetImpl() )->Bind();
}

void 
RenderSystem_GLES2_Win32::SetRenderTexture( RenderTexture* renderTexture )
{
  if( m_renderTexture != renderTexture )
  {
    renderTexture->GetImpl()->BindRenderTexture();
    m_renderTexture = renderTexture;
  }
}

void 
RenderSystem_GLES2_Win32::SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader )
{
  NE_ASSERT( vertexShader->GetImpl()->GetType() == VERTEX_SHADER, "Shader passed in the vertexShader slot is not a Vertex Shader" )();
  NE_ASSERT( fragmentShader->GetImpl()->GetType() == PIXEL_SHADER, "Shader passed in the fragmentShader slot is not a Fragement Shader" )();

  m_vertexShader   = vertexShader;
  m_fragmentShader = fragmentShader;
  m_boundProgram   = nullptr;

  // Attempt to find a ProgramObject which matches the specified shaders.
  ProgramObjectList::const_iterator programItr = m_programObjects.begin();
  for( ; programItr != m_programObjects.end(); ++programItr ) {
    if( (*programItr)->GetVertexShader() == vertexShader && (*programItr)->GetFragmentShader() == fragmentShader ) {
      m_boundProgram = (*programItr);
      break;
    }
  }

  // Check if found and if not create a new ProgramObject
  if( nullptr == m_boundProgram )
  {
    m_boundProgram = new ProgramObject( m_vertexShader, m_fragmentShader );
    m_programObjects.push_back( m_boundProgram );
    m_boundProgram->Load();
    CheckForGLESError();
  }

  NE_ASSERT( m_boundProgram, "Failed to create or find a valid Program" )( vertexShader, fragmentShader );

  // Bind the program to be used.
  m_boundProgram->UseProgram();
  CheckForGLESError();
}

void
RenderSystem_GLES2_Win32::SetOperationType( OperationType eType )
{
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
      NE_ASSERT( false, "Unknown operation type detected." )();
      break;
  }
}
    
void
RenderSystem_GLES2_Win32::Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation )
{ 
  glDrawArrays( m_OperationMode, (int)iStartVertexLocation, (int)iVertexCount );
  CheckForGLESError();
}

void
RenderSystem_GLES2_Win32::DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation )
{ 
  glDrawElements( m_OperationMode, (int)iIndexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(iStartIndexLocation) );
  CheckForGLESError();
}

void
RenderSystem_GLES2_Win32::SetBufferBinding( uint32 iTarget, uint32 iIndex, HardwareBuffer* pImpl )
{
  NE_ASSERT( false, "Not yet implemented!" )();
}

UniformDefinition 
RenderSystem_GLES2_Win32::GetUniformByName( const char* name ) const
{
  NE_ASSERT( m_boundProgram != NULL, "Expected a ProgramObject to be bound." )();

  if( NULL == m_boundProgram ) 
  {
    return UniformDefinition();
  }

  return m_boundProgram->FindUniformByName( name );
}

void
RenderSystem_GLES2_Win32::SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* sampler )
{}

void
RenderSystem_GLES2_Win32::SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* texture )
{}

void
RenderSystem_GLES2_Win32::SetUniformBinding( UniformDefinition& definition, void* value )
{
  GLint location = (GLint)definition.logicalIndex; 
  if( location == -1 ) {
    NE_ASSERT( location != -1, "Invalid uniform location found." )();
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
      int32             identifier = *static_cast<int32* >( value );
      Texture*          texture    = FindTextureByIdentifier( identifier );
      GLES2TextureImpl* impl       = static_cast<GLES2TextureImpl* >( texture->GetImpl() );

      GLuint handle = impl->GetHandle();
      glBindTexture( GL_TEXTURE_2D, handle );
      glUniform1i( location, 0 );
    } break;
  }

  CheckForGLESError();
}