#ifndef __NEBULAE_CODEC_H__
#define __NEBULAE_CODEC_H__

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

  }; //Codec

} //Nebulae

#endif //__NEBULAE_CODEC_H__