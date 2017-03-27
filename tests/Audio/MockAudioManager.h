#ifndef __NEBULAE_MOCK_AUDIOMANAGER_H__
#define __NEBULAE_MOCK_AUDIOMANAGER_H__

#include <Nebulae/Audio/AudioManager.h>

#include <gmock/gmock.h>

namespace Nebulae {

/** AudioManager.
 *  Basically represents a single "view" object in a Model-View-Controller system for 
 *  an audio engine.  All playback of audio can be executed using this class.
 */
class MockAudioManager : public AudioManager
{
  public:
    MockAudioManager( FileSystemPtr fileSystem ) : AudioManager(fileSystem) {}
    virtual ~MockAudioManager() {}

    MOCK_METHOD1( SetMasterVolume, void ( float volume ) );
    MOCK_METHOD1( CreateSound, SoundHandle ( const char* szSoundName ) );
    MOCK_METHOD0( CreateEmitter, EmitterHandle () );
    MOCK_METHOD4( AddSoundToEmitter, void ( EmitterHandle& emitter, SoundHandle& sound, const int32 loops, const Real faceTime) );
    MOCK_METHOD1( GetCurrentSound, SoundHandle ( EmitterHandle& handle ) );

    MOCK_METHOD1( Play, void ( EmitterHandle& handle ) );
    MOCK_METHOD1( Stop, void ( EmitterHandle& handle ) );
    MOCK_METHOD1( Pause, void ( EmitterHandle& handle ) );
    MOCK_METHOD1( Resume, void ( EmitterHandle& handle ) );
    MOCK_METHOD2( SetPlayCursor, void ( EmitterHandle &handle, Real time ) );

    MOCK_CONST_METHOD1( GetPlayCursor, Real ( EmitterHandle &handle ) );
    MOCK_CONST_METHOD1( GetDuration, Real ( SoundHandle sound )  );


    SoundHandle ConcreteCreateSound( const char* szSoundName ) { return AudioManager::CreateSound(szSoundName); }
    EmitterHandle ConcreteCreateEmitter() { return AudioManager::CreateEmitter(); }

}; //AudioManager

} //Nebulae

#endif // __NEBULAE_MOCK_AUDIOMANAGER_H__