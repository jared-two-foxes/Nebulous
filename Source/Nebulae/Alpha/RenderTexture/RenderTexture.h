#ifndef __NEBULAE_ALPHA_RENDERTEXTURE_H__
#define __NEBULAE_ALPHA_RENDERTEXTURE_H__

#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae {

class RenderTextureImpl;
class Texture;

class RenderTexture : public Resource
{
private:
  RenderTextureImpl* m_impl;
  Texture*           m_colour;
  Texture*           m_depth;

  public:
    /** Default Constructor. */
    RenderTexture( const std::string& name, RenderSystem* renderDevice );
    /** Destructor. */
    virtual ~RenderTexture();

    RenderTextureImpl* GetImpl() const;
    Texture*           GetColourTexture() const;
    Texture*           GetDepthTexture() const;
    std::size_t        GetWidth() const;
    std::size_t        GetHeight() const;

    void SetImpl( RenderTextureImpl* impl );
    bool BindColourBuffer( int32 index, int32 width = -1, int32 height = -1 );
    bool BindColourTexture( int32 index, Texture* texture );
    bool BindDepthBuffer( int32 width = -1, int32 height = -1 );
    bool BindDepthTexture( Texture* texture );

  private:
    virtual bool LoadImpl_( File* is ) override;
    virtual bool UnloadImpl_() override;

};

}

#endif // __NEBULAE_ALPHA_RENDERTEXTURE_H__