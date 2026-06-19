#ifndef NEBULAE_ALPHA_TEXTURE_IMAGECODEC_H_
#define NEBULAE_ALPHA_TEXTURE_IMAGECODEC_H_

#include "Nebulae/Common/Common.h"
#include "Nebulae/Alpha/Texture/Codec.h"

namespace Nebulae
{

/** Codec return class for images. Has information about the size and the
    pixel format of the image.
 */
class ImageCodecData : public CodecData
{
public:
  std::size_t height{ 0 };
  std::size_t width{ 0 };
  std::size_t size{ 0 };
  PixelFormat format{ PF_UNKNOWN };
  uint8 bpp{ 0 };
  uint8* datastream{ nullptr };

}; // ImageCodecData


class ImageCodec : public Codec
{
public:
  /** Standard Destructor.
   */
  ~ImageCodec() override {}

  /// @copydoc Codec::decode
  ImageCodecData* Decode( File& is ) const override = 0;

  /** Converts a chunk of data from its current encoding.
   */
  virtual ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalformat, std::size_t width,
                                              std::size_t height, std::size_t pitch, uint8 bpp,
                                              PixelFormat returnFormat ) const = 0;

}; // ImageCodec

} // namespace Nebulae

#endif // NEBULAE_ALPHA_TEXTURE_IMAGECODEC_H_
