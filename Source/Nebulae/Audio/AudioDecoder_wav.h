#ifndef __NEBULAE_WAVAUDIODECODER_H__
#define __NEBULAE_WAVAUDIODECODER_H__

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

    virtual bool  Parse( File* datastream ) override;
    virtual int32 Stream( uint32 start, uint32 size, char* data ) override;

};

}

#endif //  __NEBULAE_WAVAUDIODECODER_H__