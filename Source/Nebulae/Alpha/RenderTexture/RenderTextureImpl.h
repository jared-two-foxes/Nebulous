#ifndef __NEBULAE_ALPHA_RENDERTEXTUREIMPL_H__
#define __NEBULAE_ALPHA_RENDERTEXTUREIMPL_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Texture;

class RenderTextureImpl
{
public:
  int32              m_width;
  int32              m_height;
  Texture*           m_colour;
  Texture*           m_depth;

  public:
    /** Default Constructor. */
    RenderTextureImpl( int32 width, int32 height );
    /** Destructor. */
    virtual ~RenderTextureImpl();

    virtual void BindRenderTexture();
    virtual bool BindColourTexture( int32 index, Texture* texture );
    virtual bool BindColourBuffer( int32 index, int32 width = -1, int32 height = -1 );
    virtual bool BindDepthTexture( Texture* texture );
    virtual bool BindDepthBuffer( int32 index, int32 width = -1, int32 height = -1 );
    virtual bool Load();
    virtual bool Unload();

};

}

#endif // __NEBULAE_ALPHA_RENDERTEXTURE_H__