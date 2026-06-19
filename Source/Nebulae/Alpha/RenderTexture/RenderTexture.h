#ifndef NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTURE_H_
#define NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTURE_H_

#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae
{

class RenderTextureImpl;
class Texture;

class RenderTexture : public Resource
{
private:
  RenderTextureImpl* m_impl{ nullptr };
  Texture* m_colour{ nullptr };
  Texture* m_depth{ nullptr };

public:
  /** Default Constructor. */
  RenderTexture( const std::string& name, RenderSystem* renderDevice );
  /** Destructor. */
  ~RenderTexture() override;

  RenderTextureImpl* GetImpl() const;
  Texture* GetColourTexture() const;
  Texture* GetDepthTexture() const;
  std::size_t GetWidth() const;
  std::size_t GetHeight() const;

  void SetImpl( RenderTextureImpl* impl );
  bool BindColourBuffer( int32 index, int32 width = -1, int32 height = -1 );
  bool BindColourTexture( int32 index, Texture* texture );
  bool BindDepthBuffer( int32 width = -1, int32 height = -1 );
  bool BindDepthTexture( Texture* texture );

private:
  bool LoadImpl_( File* is ) override;
  bool UnloadImpl_() override;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_RENDERTEXTURE_RENDERTEXTURE_H_