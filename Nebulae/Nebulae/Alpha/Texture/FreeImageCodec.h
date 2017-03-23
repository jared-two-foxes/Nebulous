#ifndef __NEBULAE_FREEIMAGECODEC_H__
#define __NEBULAE_FREEIMAGECODEC_H__

#include <Nebulae/Alpha/Texture/ImageCodec.h>

namespace Nebulae {
  
class FreeImageCodec : public ImageCodec {
  
  public:
    FreeImageCodec();
    virtual ~FreeImageCodec();

    /// @copydoc Codec::decode
    virtual ImageCodecData* Decode( File& is ) const override;

    virtual ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalFormat, std::size_t width, std::size_t height, std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const override;

}; //Codec

} //Nebulae

#endif //__NEBULAE_CODEC_H__