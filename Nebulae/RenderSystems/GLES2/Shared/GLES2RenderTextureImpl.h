#ifndef __NEBULAE_ALPHA_GLES2_RENDERTEXTUREIMPL_H__
#define __NEBULAE_ALPHA_GLES2_RENDERTEXTUREIMPL_H__

#include <GLES2/config.h>

#include <Nebulae/Alpha/RenderTexture/RenderTextureImpl.h>

namespace Nebulae
{

class GLES2RenderBuffer;

class GLES2RenderTextureImpl : public RenderTextureImpl
{
public:
  GLuint             m_framebuffer;
  GLES2RenderBuffer* m_colourBuffer;
  GLES2RenderBuffer* m_depthBuffer;

  public:
    GLES2RenderTextureImpl( int32 width, int32 height );
    ~GLES2RenderTextureImpl();

    virtual void BindRenderTexture() override;
    virtual bool BindColourTexture( int32 index, Texture* texture ) override;
    virtual bool BindColourBuffer( int32 index, int32 width = -1, int32 height = -1 ) override;
    virtual bool BindDepthTexture( Texture* texture ) override;
    virtual bool BindDepthBuffer( int32 index, int32 width = -1, int32 height = -1 ) override;
    virtual bool Load() override;
    virtual bool Unload() override;

};

}

#endif // __NEBULAE_ALPHA_GLES2_RENDERTEXTUREIMPL_H__