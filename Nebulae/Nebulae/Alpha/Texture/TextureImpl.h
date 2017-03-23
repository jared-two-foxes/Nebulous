#ifndef NEBULAE_ALPHA_TEXTUREIMPL_H__
#define NEBULAE_ALPHA_TEXTUREIMPL_H__

#include <Nebulae/Alpha/Texture/Texture.h>

namespace Nebulae {

class TextureImpl {

  protected:
    std::string m_filename;
    TextureType m_type;
    PixelFormat m_pixelFormat;
    std::size_t m_width;
    std::size_t m_height;
    std::size_t m_depth;

    public:
      TextureImpl( std::string strFilename );
      virtual ~TextureImpl();

      //getters
      TextureType GetTextureType() const;
      PixelFormat GetFormat() const;
      std::size_t GetWidth() const;
      std::size_t GetHeight() const;
      std::size_t GetDepth() const;

      //functions
      virtual bool Load( const Image& image );
      virtual bool LoadFromMemory( unsigned char* szBuffer, uint8 srcDataType, uint8 channels, int iWidth, int iHeight );
      virtual bool Unload();

};

}

#endif // NEBULAE_ALPHA_TEXTURE_H__