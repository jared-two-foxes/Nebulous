#ifndef __AUDIOCONTROLLER_H__
#define __AUDIOCONTROLLER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Audio/Audio.h>

namespace Nebulae {
  class AudioManager;
}

namespace Game {
  
class AudioController
///
/// The AudioController is responsible for handling any audio requests that the game
/// may have this includes adjusting playback volume, creation & management of music 
/// and sfx.
///
{
private:
  typedef std::pair<std::string, Nebulae::EmitterHandle > NamedEmitter;

private:
  Nebulae::AudioManager&     m_audioManager;
  std::vector<NamedEmitter > m_sfxEmitters;
  std::vector<NamedEmitter > m_musicEmitters;

  public:
    explicit AudioController( Nebulae::AudioManager& audioManager );
    ~AudioController();

    void Clear();
    void Update( const float elapsed );

    Real GetMasterVolume() const;
    bool IsNameValid( const char* identifier ) const;
    bool IsPlaying( const char* identifier ) const;
    bool IsPaused( const char* identifier ) const;

    void SetMasterVolume( Real volume );

    void PlaySfx( const char* identifier, const char* filename, int32 loops = 0 );
    void StopSfx( const char* identifier );
    
    void PlayMusic( const char* identifier, const char* filename, int32 loops = -1 );
    void PauseMusic( const char* identifier );
    void ResumeMusic( const char* identifier );
    void StopMusic( const char* identifier );

  private:
    const Nebulae::EmitterHandle* FindNamedEffectEmitter( const char* filename ) const;
    const Nebulae::EmitterHandle* FindNamedMusicEmitter( const char* filename ) const;

};

}

#endif //__AUDIOCONTROLLER_H__