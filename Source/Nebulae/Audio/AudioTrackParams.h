#ifndef NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_
#define NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
struct AudioTrackParams
{
  int32 numChannels{ 0 };
  int32 samplingRate{ 0 };
  int32 bitsPerSample{ 0 };
  uint32 numSamples{ 0 };
  uint64 m_dataSize{ 0 };
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_
