
#include "GLES2RenderBuffer.h"
#include "GLES2TextureImpl.h"

#include <Nebulae/Alpha/Texture/Texture.h>

using namespace Nebulae;

GLES2RenderBuffer::GLES2RenderBuffer()
: m_handle(0)
{}

GLES2RenderBuffer::~GLES2RenderBuffer()
{}

GLuint 
GLES2RenderBuffer::GetHandle() const
{
  return m_handle;
}

void 
GLES2RenderBuffer::Bind()
{
  glBindRenderbuffer( GL_RENDERBUFFER, m_handle );
}

bool 
GLES2RenderBuffer::Load()
{
  glGenRenderbuffers( 1, &m_handle );
  return true;
}

bool 
GLES2RenderBuffer::Unload()
{ 
  glDeleteRenderbuffers( 1, &m_handle );
  return false; 
}

  
  // // Get the storage from iOS so it can be displayed in the view.
  // [m_pContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)m_pViewController.view.layer];
  
  // // Get the frames width and height
  // int framebufferwidth, framebufferheight;
  // glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferwidth );
  // glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferheight );
  
  // glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorrenderbuffer );
  
  // // Create a depth renderbuffer
  // glGenRenderbuffers( 1, &m_depthrenderbuffer );
  // glBindRenderbuffer( GL_RENDERBUFFER, m_depthrenderbuffer );
  // // Create the storeage for the buffer, optimized for depth values, same size as the colorrenderbuffer
  // glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferwidth, framebufferheight );
  // // Attach the depth buffer for our framebuffer
  // glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthrenderbuffer );