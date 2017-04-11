#ifndef __NEBULAE_GLI_CODEC_H__
#define __NEBULAE_GLI_CODEC_H__

#include "Nebulae/Alpha/Texture/ImageCodec.h"

namespace Nebulae {

class GliCodec : public ImageCodec
{
public:
  /** Standard Constructor.
   */  
  GliCodec();

  /** Standard Destructor.
   */
  virtual ~GliCodec();

  /// @copydoc Codec::decode
  virtual ImageCodecData* Decode( File& is ) const override;

  /** Converts a chunk of data from its current encoding.
    */
  virtual ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalformat, std::size_t width, std::size_t height, std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const ;
  
}; //ImageCodec

}

#endif //__NEBULAE_GLI_CODEC_H__