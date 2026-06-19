#ifndef NEBULAE_ALPHA_TEXTURE_CODEC_H_
#define NEBULAE_ALPHA_TEXTURE_CODEC_H_

#include "Nebulae/Common/Base/Base.h"

namespace Nebulae
{

class CodecData
{
public:
  virtual ~CodecData() {}
};

/** Serialisation class.
 */
class Codec
{
public:
  virtual ~Codec() {}

  /** Codes the data from the input chunk into the output chunk.
      @param
        input Stream containing the encoded data
  */
  virtual CodecData* Decode( File& is ) const = 0;

}; // Codec

} // namespace Nebulae

#endif // NEBULAE_ALPHA_TEXTURE_CODEC_H_