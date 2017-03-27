#ifndef __NEBULAE_OPENAUDIOLIBRARYEMITTER_H__
#define __NEBULAE_OPENAUDIOLIBRARYEMITTER_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioEmitterInterface.h>

namespace Nebulae
{

  ///
  /// An object that represents a playing sound.  Attach an AudioBuffer to play said buffer.
  ///
  class OpenAudioLibraryEmitter : public AudioEmitterInterface
  {
  private:
    uint32 m_id; ///< Internal handle to a hardware source implmentation.
    
    public:
      OpenAudioLibraryEmitter( AudioBufferInterface** streamBuffers, int count );
      virtual ~OpenAudioLibraryEmitter();

      virtual bool Initiate() override;
      virtual void Destroy() override;

      virtual void Play() override;
      virtual void Stop() override;
      virtual void Pause() override;
      virtual void Resume() override;
    
      virtual bool IsPlaying() const override;
      virtual bool IsPaused() const override;

    private:
      virtual int  _BufferProcessed() const override;
      virtual bool _EnqueueBuffer( AudioBufferInterface* sound ) override;
      virtual bool _UnqueueBuffer( AudioBufferInterface* sound ) override;

  };

}

#endif // __NEBULAE_OPENAUDIOLIBRARYEMITTER_H__