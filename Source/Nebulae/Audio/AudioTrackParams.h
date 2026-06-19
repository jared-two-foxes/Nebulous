#ifndef NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_
#define NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
struct AudioTrackParams
{
  int32 numChannels;
  int32 samplingRate;
  int32 bitsPerSample;
  uint32 numSamples;
  uint64 m_dataSize;

  AudioTrackParams() : numChannels( 0 ), samplingRate( 0 ), bitsPerSample( 0 ), numSamples( 0 ), m_dataSize( 0 ) {}
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_AUDIOTRACKPARAMS_H_