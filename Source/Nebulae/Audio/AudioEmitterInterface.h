#ifndef __NEBULAE_AUDIOEMITTERINTERFACE_H__
#define __NEBULAE_AUDIOEMITTERINTERFACE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

  class AudioBufferInterface;
  class AudioSoundInterface;

  ///
  /// An object that represents a location in space from which sound can be generated.  
  /// Sounds are played by binding, or queueing mutliple, AudioSoundInterface to it.
  ///
  class AudioEmitterInterface
  {
  public:
    struct QueuedEmitterSound
    {
      AudioSoundInterface* sound;
      int                  loops;
    };

  protected:
    std::list<QueuedEmitterSound >     m_queuedSounds;       ///< Queue of sounds to play on this channel.
    std::list<AudioBufferInterface* >  m_buffers;            ///< The Buffers that this emitter uses to stream audio.
    std::list<AudioBufferInterface* >  m_queued_buffers;     ///< The Buffers that are currently filled with audio.
    uint64                             m_total_buffered;     ///< The amount of the current sound that has been streamed.
    
    public:
      AudioEmitterInterface( AudioBufferInterface** streamBuffers, int count );
      virtual ~AudioEmitterInterface();

      AudioSoundInterface* GetPlayingSound() const;
      
      virtual bool Initiate() = 0;
      virtual void Destroy() = 0;
      virtual void Update( const Real elapsed );

      virtual void Play() = 0;
      virtual void Stop() = 0;
      virtual void Pause() = 0;
      virtual void Resume() = 0;

      virtual Real GetPlayCursor() const;
      virtual void SetPlayCursor( Real time );

      virtual bool IsPlaying() const = 0;
      virtual bool IsPaused() const = 0;

      virtual void Push( AudioSoundInterface* sound, int32 loops = 0, bool force = true );
      virtual void Pop();

    protected:
      bool StreamBuffer();

      virtual int  _BufferProcessed() const = 0;
      virtual bool _EnqueueBuffer( AudioBufferInterface* buffer ) = 0;
      virtual bool _UnqueueBuffer( AudioBufferInterface* buffer ) = 0;

  };

}

#endif // __NEBULAE_AUDIOEMITTERINTERFACE_H__