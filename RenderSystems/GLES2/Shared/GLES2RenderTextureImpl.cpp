
#include "GLES2RenderTextureImpl.h"
#include "GLES2RenderBuffer.h"
#include "GLES2TextureImpl.h"

#include <Nebulae/Alpha/Texture/Texture.h>

using namespace Nebulae;

GLES2RenderTextureImpl::GLES2RenderTextureImpl( int32 width, int32 height )
: RenderTextureImpl(width, height),
  m_framebuffer(0),
  m_colourBuffer(nullptr),
  m_depthBuffer(nullptr)
{}

GLES2RenderTextureImpl::~GLES2RenderTextureImpl()
{
  if( m_colourBuffer != nullptr )
  {
    m_colourBuffer->Unload();
    delete m_colourBuffer;
    m_colourBuffer = nullptr;
  }

  if( m_depthBuffer != nullptr )
  {
    m_depthBuffer->Unload();
    delete m_depthBuffer;
    m_depthBuffer = nullptr;
  }
}

void 
GLES2RenderTextureImpl::BindRenderTexture()
{
  glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffer );
}

bool 
GLES2RenderTextureImpl::BindColourBuffer( int32 index, int32 width, int32 height )
{
  return false;
}

bool 
GLES2RenderTextureImpl::BindColourTexture( int32 index, Texture* texture )
{ 
  NE_ASSERT( m_colourBuffer == nullptr, "Attempting to set a texture to a colour slot which is already occupied" )( index );

  m_colourBuffer = new GLES2RenderBuffer();
  m_colourBuffer->Load(); //glGenRenderbuffers( 1, &m_colourRenderBuffer );
  m_colourBuffer->Bind(); //glBindRenderbuffer( GL_RENDERBUFFER, m_colourRenderBuffer );

  //Create Frame Buffer
  if( texture->GetTextureType() == TextureType::TEX_TYPE_CUBE_MAP )
  {
    //@todo handle binding a cube map texture
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + pRenderTexture->face, pRenderTexture->color->handle, 0);
  }
  else
  {
    GLuint textureHandle = static_cast<GLES2TextureImpl*>(texture->GetImpl())->GetHandle();
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0 );
  }

  return false; 
}

bool 
GLES2RenderTextureImpl::BindDepthBuffer( int32 index, int32 width, int32 height )
{
  NE_ASSERT( m_depthBuffer == nullptr, "Attempting to set a texture to the depth slot which is already occupied" )();

  m_depthBuffer = new GLES2RenderBuffer();
  m_depthBuffer->Load();
  m_depthBuffer->Bind();

  // Create the storeage for the buffer, optimized for depth values
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height );

  // Attach the depth buffer for our framebuffer
  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer->GetHandle() );

  //@todo check GLES errors?

  return true;
}

bool 
GLES2RenderTextureImpl::BindDepthTexture( Texture* texture )
{ 
  NE_ASSERT( m_depthBuffer == nullptr, "Attempting to set a texture to the depth slot which is already occupied" )();
  NE_ASSERT( texture->GetTextureType() == TextureType::TEX_TYPE_2D, "Attempting to set a texture to the depth slot which is the incorrect format" )();

  m_depthBuffer = new GLES2RenderBuffer();
  m_depthBuffer->Load();
  m_depthBuffer->Bind();

  GLuint textureHandle = static_cast<GLES2TextureImpl*>(texture->GetImpl())->GetHandle();
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureHandle, 0 );

  //@todo check GLES errors?

  return true;
}

bool 
GLES2RenderTextureImpl::Load()
{
  glGenFramebuffers( 1, &m_framebuffer );
  return false;
}

bool 
GLES2RenderTextureImpl::Unload()
{ 
  glDeleteFramebuffers( 1, &m_framebuffer );
  return false; 
}