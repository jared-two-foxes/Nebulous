#ifndef __NEBULAE_IMAGECODEC_H__
#define __NEBULAE_IMAGECODEC_H__

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
  ImageCodecData() : height(0), width(0), size(0), format(PF_UNKNOWN), bpp(0), datastream(NULL) {}

  std::size_t height;
  std::size_t width;
  std::size_t size;
  PixelFormat format;
  uint8       bpp;
  uint8*      datastream;

}; // ImageCodecData


class ImageCodec : public Codec
{
public:
  /** Standard Destructor.
   */
  virtual ~ImageCodec() {}

  /// @copydoc Codec::decode
  virtual ImageCodecData* Decode( File& is ) const override
  { return NULL; }

  /** Converts a chunk of data from its current encoding.
    */
  virtual ImageCodecData* ConvertFromRawBits( uint8* bits, PixelFormat originalformat, std::size_t width, std::size_t height, std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const 
  { return NULL; }
  
}; //ImageCodec

}

#endif //__NEBULAE_IMAGECODEC_H__