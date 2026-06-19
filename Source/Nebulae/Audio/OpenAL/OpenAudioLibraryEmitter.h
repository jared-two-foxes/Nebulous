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
  ~OpenAudioLibraryEmitter() override;

  bool Initiate() override;
  void Destroy() override;

  void Play() override;
  void Stop() override;
  void Pause() override;
  void Resume() override;

  bool IsPlaying() const override;
  bool IsPaused() const override;

private:
  int _BufferProcessed() const override;
  bool _EnqueueBuffer( AudioBufferInterface* sound ) override;
  bool _UnqueueBuffer( AudioBufferInterface* sound ) override;
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_OPENAL_OPENAUDIOLIBRARYEMITTER_H_