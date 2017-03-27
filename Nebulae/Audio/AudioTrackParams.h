#ifndef __NEBULAE_AUDIOTRACKPARAMS_H__
#define __NEBULAE_AUDIOTRACKPARAMS_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  struct AudioTrackParams
  {
    int32   numChannels;
    int32   samplingRate;
    int32   bitsPerSample;
    uint32  numSamples;
    uint64  m_dataSize;

    AudioTrackParams() : numChannels(0), samplingRate(0), bitsPerSample(0), numSamples(0), m_dataSize(0) {}
  };

}

#endif // __NEBULAE_AUDIOTRACKPARAMS_H__