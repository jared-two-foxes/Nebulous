#ifndef NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTUREIMPL_H_
#define NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTUREIMPL_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Texture;

class RenderTextureImpl
{
public:
  int32 m_width;
  int32 m_height;
  Texture* m_colour{ nullptr };
  Texture* m_depth{ nullptr };

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

} // namespace Nebulae

#endif // NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTUREIMPL_H_