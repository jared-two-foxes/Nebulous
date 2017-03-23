
#include "AudioController.h"

#include <Nebulae/Audio/Audio.h>


using namespace Nebulae;
using namespace Game;


AudioController::AudioController( Nebulae::AudioManager& audioManager )
///
/// Default Contructor
///
  : m_audioManager( audioManager )
{}


AudioController::~AudioController()
{
  Clear();
}


void
AudioController::Clear()
{
  std::for_each( m_sfxEmitters.begin(), m_sfxEmitters.end(), [&](NamedEmitter& emitter){
    m_audioManager.Stop( emitter.second );
  }); 
  
  std::for_each( m_musicEmitters.begin(), m_musicEmitters.end(), [&](NamedEmitter& emitter){
    m_audioManager.Stop( emitter.second );
  });
}


Real 
AudioController::GetMasterVolume() const
{ 
  return m_audioManager.GetMasterVolume(); 
}


bool 
AudioController::IsNameValid( const char* identifier ) const
{
  return FindNamedEffectEmitter(identifier) != nullptr 
         || FindNamedMusicEmitter(identifier) != nullptr;
}

bool 
AudioController::IsPlaying( const char* identifier ) const
{
  const Nebulae::EmitterHandle* handle = FindNamedEffectEmitter( identifier );
  if( handle == nullptr ) handle = FindNamedMusicEmitter( identifier );
  if( handle == nullptr ) return false;

  return m_audioManager.IsPlaying( *handle );
}

bool 
AudioController::IsPaused( const char* identifier ) const
{
  const Nebulae::EmitterHandle* handle = FindNamedEffectEmitter( identifier );
  if( handle == nullptr ) handle = FindNamedMusicEmitter( identifier );
  if( handle == nullptr ) return false;

  return m_audioManager.IsPaused( *handle );
}


void 
AudioController::SetMasterVolume( Real volume )
{ 
  m_audioManager.SetMasterVolume( volume ); 
}


void 
AudioController::PlaySfx( const char* identifier, const char* filename, int32 loops )
{
  // Early out if not filename is specified.
  if( filename == NULL || strlen(filename) == 0 ) return;

  SoundHandle   sound   = m_audioManager.CreateSound( filename );
  EmitterHandle emitter = m_audioManager.CreateEmitter();

  m_musicEmitters.push_back( std::make_pair(identifier,emitter) );

  m_audioManager.AddSoundToEmitter( emitter, sound, loops );

  m_audioManager.Play( emitter );
}


void 
AudioController::StopSfx( const char* identifier )
{
  Nebulae::EmitterHandle* handle = const_cast<Nebulae::EmitterHandle*>( FindNamedEffectEmitter(identifier) );
  if( handle != nullptr )
  {
    m_audioManager.Stop( *handle );
  }
}


void 
AudioController::PlayMusic( const char* identifier, const char* filename, int32 loops )
{
  // Early out if not filename is specified.
  if( filename == NULL || strlen(filename) == 0 ) return;
  
  SoundHandle   sound   = m_audioManager.CreateSound( filename );
  EmitterHandle emitter = m_audioManager.CreateEmitter();

  m_musicEmitters.push_back( std::make_pair(identifier,emitter) );

  m_audioManager.AddSoundToEmitter( emitter, sound, loops );

  m_audioManager.Play( emitter );
}


void
AudioController::PauseMusic( const char* identifier )
{
  Nebulae::EmitterHandle* handle = const_cast<Nebulae::EmitterHandle*>( FindNamedMusicEmitter(identifier) );
  if( handle != nullptr )
  {
    m_audioManager.Pause( *handle );
  } 
}


void 
AudioController::ResumeMusic( const char* identifier )
{
  Nebulae::EmitterHandle* handle = const_cast<Nebulae::EmitterHandle*>( FindNamedMusicEmitter(identifier) );
  if( handle != nullptr )
  {
    m_audioManager.Resume( *handle );
  } 
}


void 
AudioController::StopMusic( const char* identifier )
{
  Nebulae::EmitterHandle* handle = const_cast<Nebulae::EmitterHandle*>( FindNamedMusicEmitter(identifier) );
  if( handle != nullptr )
  {
    m_audioManager.Stop( *handle );
  }
}


void
AudioController::Update( const float elapsed )
{
  //PROFILE;

  m_audioManager.Update( elapsed );

//
// Iterate any sfx emitters and remove them if they have finished playing.
//
  for( std::vector<NamedEmitter >::iterator it = m_sfxEmitters.begin(); it != m_sfxEmitters.end(); )
  {
    if( !m_audioManager.IsPlaying(it->second) )
    {
      m_sfxEmitters.erase(it);
    }
    else
    {
      ++it;
    }
  }
}


const Nebulae::EmitterHandle*
AudioController::FindNamedEffectEmitter( const char* identifier ) const
{
  std::vector<NamedEmitter >::const_iterator iterator = std::find_if( m_sfxEmitters.begin(), m_sfxEmitters.end(), [&](const NamedEmitter& ne) {
    return strcmp(ne.first.c_str(), identifier) == 0; 
  });

  if( iterator != m_sfxEmitters.end() )
  {
    return &iterator->second; 
  }

  return nullptr;
}


const Nebulae::EmitterHandle*
AudioController::FindNamedMusicEmitter( const char* identifier ) const
{
  std::vector<NamedEmitter >::const_iterator iterator = std::find_if( m_musicEmitters.begin(), m_musicEmitters.end(), [&](const NamedEmitter& ne) {
  	return strcmp(ne.first.c_str(), identifier) == 0; 
  });

  if( iterator != m_sfxEmitters.end() )
  {
    return &iterator->second;
  }

  return nullptr;
}