#ifndef __NEBULAE_AUDIOMANAGER_H__
#define __NEBULAE_AUDIOMANAGER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  
//Forward Decelerations
class AudioBackend;
class SoundHandle;
class EmitterHandle;

/** AudioManager.
 *  Basically represents a single "view" object in a Model-View-Controller system for 
 *  an audio engine.  All playback of audio can be executed using this class.
 */
class AudioManager
{
public:
  typedef std::shared_ptr<FileSystem > FileSystemPtr;

private:
  FileSystemPtr m_fileSystem;
  AudioBackend* m_pAudioBackend;

  public:
    AudioManager( FileSystemPtr fileSystem );
    virtual ~AudioManager();

    bool  Initiate();
    void  Destroy();
    void  Update( const float elapsed );

    virtual void  SetMasterVolume( float volume );
    virtual float GetMasterVolume() const;

    virtual SoundHandle CreateSound( const char* szSoundName );

    virtual Real GetDuration( SoundHandle sound ) const;

    virtual EmitterHandle CreateEmitter();

    
    virtual void AddSoundToEmitter( EmitterHandle& emitter, SoundHandle& sound, const int32 loops = 0, const Real faceTime = 0.0f );

    virtual SoundHandle GetCurrentSound( EmitterHandle& handle );
    
    virtual void  Play( EmitterHandle& handle );
    virtual void  Stop( EmitterHandle& handle );
    virtual void  Pause( EmitterHandle& handle );
    virtual void  Resume( EmitterHandle& handle );

    /** Return the current playback position of a cursor. */
    virtual Real GetPlayCursor( EmitterHandle &handle ) const;

    /** Set the current playback cursor position. */
    virtual void SetPlayCursor( EmitterHandle &handle, Real time = 0.0f );

    bool IsReady( const EmitterHandle& emitter ) const;

    bool IsValid( const EmitterHandle& emitter ) const;

    bool IsAlive( const EmitterHandle& emitter ) const;

    /** Test if an emitter is currently playing. */
    bool IsPlaying( const EmitterHandle& handle ) const;

    /** Test if an emitter is currently paused. */
    bool IsPaused( const EmitterHandle& handle ) const;

}; //AudioManager

} //Nebulae

#endif // __NEBULAE_AUDIOMANAGER_H__