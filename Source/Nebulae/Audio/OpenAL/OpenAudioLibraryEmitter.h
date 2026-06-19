#ifndef NEBULAE_AUDIO_OPENAL_OPENAUDIOLIBRARYEMITTER_H_
#define NEBULAE_AUDIO_OPENAL_OPENAUDIOLIBRARYEMITTER_H_

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
  virtual int _BufferProcessed() const override;
  virtual bool _EnqueueBuffer( AudioBufferInterface* sound ) override;
  virtual bool _UnqueueBuffer( AudioBufferInterface* sound ) override;
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_OPENAL_OPENAUDIOLIBRARYEMITTER_H_