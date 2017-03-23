#ifndef __NEBULAE_AUDIO_BUFFERINTERFACE_H__
#define __NEBULAE_AUDIO_BUFFERINTERFACE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  
  enum AudioFormat
  {
    AUDIO_FORMAT_MONO8,     ///< Single channel, 8 bit data
    AUDIO_FORMAT_MONO16,    ///< Single channel, 16 bit data

    AUDIO_FORMAT_STEREO8,   ///< 2 channels, 8 bit data
    AUDIO_FORMAT_STEREO16   ///< 2 channels, 16 bit data
  };

  ///
  /// Abstract class used to define an interface to a buffer class that is used
  /// to store audio data for playback.
  ///
  class AudioBufferInterface
  {
  public:
    virtual ~AudioBufferInterface() {}

    virtual bool Initiate() = 0;
    virtual bool Fill( AudioFormat format, void* data, uint32 iSize, uint32 frequency ) = 0;
    virtual bool Destroy() = 0;

  }; //AudioBufferInterface

}

#endif // __NEBULAE_AUDIO_BUFFERINTERFACE_H__