#ifndef NEBULAE_AUDIO_AUDIODECODER_WAV_H_
#define NEBULAE_AUDIO_AUDIODECODER_WAV_H_

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioDecoder.h>

namespace Nebulae
{

class WavAudioDecoder : public AudioDecoder
{
private:
  std::size_t m_dataSize;
  std::size_t m_dataOffset;

public:
  WavAudioDecoder();
  virtual ~WavAudioDecoder() {}

  virtual bool Parse( File* datastream ) override;
  virtual int32 Stream( uint32 start, uint32 size, char* data ) override;
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_AUDIODECODER_WAV_H_