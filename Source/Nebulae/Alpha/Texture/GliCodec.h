#ifndef NEBULAE_ALPHA_TEXTURE_GLICODEC_H_
#define NEBULAE_ALPHA_TEXTURE_GLICODEC_H_

#include "Nebulae/Alpha/Texture/ImageCodec.h"

namespace Nebulae
{

class GliCodec : public ImageCodec
{
public:
  /** Standard Constructor.
   */
  GliCodec();

  /** Standard Destructor.
   */
  ~GliCodec() override;

  /// @copydoc Codec::decode
  ImageCodecData* Decode( File& is ) const override;

  /** Converts a chunk of data from its current encoding.
   */
  ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalformat, std::size_t width, std::size_t height,
                                      std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const override;

}; // ImageCodec

} // namespace Nebulae

#endif // NEBULAE_ALPHA_TEXTURE_GLICODEC_H_