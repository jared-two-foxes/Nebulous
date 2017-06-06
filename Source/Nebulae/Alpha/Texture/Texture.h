#ifndef NEBULAE_ALPHA_TEXTURE_H__
#define NEBULAE_ALPHA_TEXTURE_H__

#include <Nebulae/Alpha/Resource/Resource.h>
#include <Nebulae/Alpha/Texture/Image.h>

namespace Nebulae {

class TextureImpl;

enum class TextureType
{
	UNKNOWN = 0,
	TEX_TYPE_1D,
	TEX_TYPE_2D, 
	TEX_TYPE_3D, 
	TEX_TYPE_CUBE_MAP
}; 


/** Texture.
	*/
class Texture : public Resource
{
protected:
  Image        m_image;
	TextureImpl* m_impl;
		
	public:
		/** Default Constructor. */
		Texture( const std::string& name, RenderSystem* renderDevice );
		/** Destructor. */
		virtual ~Texture();

		TextureImpl* GetImpl() const;
		TextureType  GetTextureType() const;
    PixelFormat  GetFormat() const;
		std::size_t  GetWidth() const;
		std::size_t  GetHeight() const;
		std::size_t  GetDepth() const;


    /** Return hardware pixel buffer for a surface. This buffer can then
        be used to copy data from and to a particular level of the texture.
        @param face 
          Face number, in case of a cubemap texture. Must be 0 for other 
          types of textures. For cubemaps, this is one of 
            +X (0), -X (1), +Y (2), -Y (3), +Z (4), -Z (5)
        @param mipmap 
          Mipmap level. This goes from 0 for the first, largest mipmap level
          to GetNumMipmaps()-1 for the smallest.
        @return 
            A shared pointer to a hardware pixel buffer
        @remarks  
          The buffer is invalidated when the resource is unloaded or destroyed.
          Do not use it after the lifetime of the containing texture.
    */
    //virtual HardwarePixelBufferSharedPtr GetBuffer(size_t face=0, size_t mipmap=0) = 0;


    void SetImpl( TextureImpl* impl );
    void ConvertToImage( Image& image, bool includeMipMaps = false ) const;
		bool LoadFromMemory( unsigned char* data, uint8 srcDataType, uint8 channels, int width, int height );

  private:
    virtual bool LoadImpl_( File* is ) override;
    virtual bool UnloadImpl_() override;

}; //Texture

} //Nebulae

#endif // TEXTURE_H__