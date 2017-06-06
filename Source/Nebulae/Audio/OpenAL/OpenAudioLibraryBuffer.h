#ifndef __NEBULAE_AUDIOBUFFER_H__
#define __NEBULAE_AUDIOBUFFER_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioBufferInterface.h>

#include <Nebulae/Audio/OpenAL/CommonHeaders.h>

namespace Nebulae 
{

  ///
  /// A utility class to assist interactions with audio buffers.
  ///
  class OpenAudioLibraryBuffer : public AudioBufferInterface
  {
    friend class OpenAudioLibraryEmitter;

  public:

    OpenAudioLibraryBuffer( uint32 size );
    virtual ~OpenAudioLibraryBuffer();

    uint32 GetHandle() const { return m_id; }

    virtual bool Initiate() override;
    virtual bool Fill( AudioFormat format, void* data, uint32 iSize, uint32 frequency ) override;
    virtual bool Destroy() override;

  private:

    /// Internal handle to a hardware buffer containing the sound data.
    uint32 m_id;



    // We could possibly store this stuff?
    //struct internal {
    //  uint32 m_size;
    //  uint32 m_frequency; 
    //};

  }; //OpenAudioLibraryBuffer

}

#endif // __NEBULAE_AUDIOBUFFER_H__