#ifndef __NEBULAE_AUDIOBACKEND_H__
#define __NEBULAE_AUDIOBACKEND_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class AudioDriver;
class AudioEmitterInterface;
class AudioSoundInterface;
class EmitterHandle;
class SoundHandle;

/** Internal workings of Audio playback & control.
 *  Basically a wrapper to the AudioDriver interface, in most case (currently ALL cases) this defaults to OpenAL.
 */
class AudioBackend
{
public:
  typedef std::shared_ptr<FileSystem > FileSystemPtr;

private:
  FileSystemPtr                        m_fileSystem; ///< The file system access object.
  AudioDriver*                         m_pDriver;    ///< Reference to the object that is currently being used to implement sound on hardware. 
  std::vector<AudioEmitterInterface* > m_emitters;   ///< List of all emitter objects in the audio landscape.
  std::vector<AudioSoundInterface* >   m_sounds;     ///< List of all sounds that the audio system is currently aware of.
  float                                m_volume;     ///< Master volume of audio universe, [0.0 - 1.0] 

  public:
    AudioBackend( FileSystemPtr fileSystem );
    virtual ~AudioBackend();

    bool Initiate();
    void Update( const float elapsed );
    void Destroy();

    SoundHandle CreateSound( const char* filename );

    Real GetDuration( SoundHandle sound ) const;

    EmitterHandle CreateEmitter();

    void AddSoundToEmitter( EmitterHandle& emitter, SoundHandle& sound, const int32 loops, const Real fadeTime );

    SoundHandle GetCurrentSound( EmitterHandle& handle );
    
    void  Play( EmitterHandle& handle, const Real fadeTime );
    void  Stop( EmitterHandle& handle, const Real fadeTime );
    void  Pause( EmitterHandle& handle, const Real fadeTime );
    void  Resume( EmitterHandle& handle, const Real fadeTime );

    Real  GetPlayCursor( EmitterHandle& handle ) const;
    void  SetPlayCursor( EmitterHandle& handle, Real time = 0.0f );

    bool  IsPlaying( const EmitterHandle& handle ) const;
    bool  IsPaused( const EmitterHandle& handle ) const;

    void  SetMasterVolume( float volume );
    float GetMasterVolume() const;

}; //AudioBacked

} //Nebulae

#endif // __NEBULAE_AUDIOBACKEND_H__

