#ifndef NEBULAE_ALPHA_TEXTURE_FREEIMAGECODEC_H_
#define NEBULAE_ALPHA_TEXTURE_FREEIMAGECODEC_H_

#include "Nebulae/Alpha/Texture/ImageCodec.h"

namespace Nebulae
{

class FreeImageCodec : public ImageCodec
{
public:
  /** Standard Constructor.
   */
  FreeImageCodec();

  /** Standard Destructor.
   */
  virtual ~FreeImageCodec();

  /// @copydoc Codec::decode
  virtual ImageCodecData* Decode( File& is ) const override;

  /** Converts a chunk of data from its current encoding.
   */
  virtual ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalformat, std::size_t width,
                                              std::size_t height, std::size_t pitch, uint8 bpp,
                                              PixelFormat returnFormat ) const;

}; // ImageCodec

} // namespace Nebulae

#endif // NEBULAE_ALPHA_TEXTURE_FREEIMAGECODEC_H_