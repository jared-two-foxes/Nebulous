
#include "AudioManager.h"

#include <Nebulae/Audio/AudioBackend.h>
#include <Nebulae/Audio/EmitterHandle.h>
#include <Nebulae/Audio/SoundHandle.h>

using namespace Nebulae;


AudioManager::AudioManager( FileSystemPtr fileSystem )
: m_fileSystem( fileSystem ),
  m_pAudioBackend( NULL )
{
    
}


AudioManager::~AudioManager()
{
  Destroy();
}


bool 
AudioManager::Initiate()
///
/// Initializes the Audio System including creating & setting up instance driver
///
/// @returns
///   Success of the initialization.
///
{
  m_pAudioBackend = new AudioBackend( m_fileSystem );
  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Initiate();

    return true;
  }

  return false;
}


void 
AudioManager::Destroy()
///
/// Tear down the Audio system including driver and resources
///
/// @return
///   Nothing.
///
{
  if( m_pAudioBackend != NULL )
  {
    delete m_pAudioBackend;
  }
  m_pAudioBackend = 0;
}


void 
AudioManager::Update( const Real elapsed )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Update( elapsed );
  }
}


SoundHandle 
AudioManager::CreateSound( const char* szSoundName )
/// @todo [jared.watt 04.04.2013]
///   Check if this sound already exists, if so return reference to that sound?
{
  NE_ASSERT( m_pAudioBackend, "" )();

  SoundHandle handle;
  if( m_pAudioBackend != NULL ) 
  {
    handle = m_pAudioBackend->CreateSound( szSoundName );
  }

  return handle;
}


Real
AudioManager::GetDuration( SoundHandle sound ) const
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL )
  {
    return m_pAudioBackend->GetDuration( sound );
  }

  return 0.0f; //@todo [jared.watt] Should maybe return std::numeric_limits<float>::max()
}


EmitterHandle 
AudioManager::CreateEmitter()
{
  NE_ASSERT( m_pAudioBackend, "" )();

  EmitterHandle handle;
  if( m_pAudioBackend != NULL ) 
  {
    handle = m_pAudioBackend->CreateEmitter();
  }
  return handle;
}


SoundHandle 
AudioManager::GetCurrentSound( EmitterHandle& handle )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  SoundHandle sound;
  if( m_pAudioBackend != NULL ) 
  {
    sound = m_pAudioBackend->GetCurrentSound( handle );
  }
  return sound;
}


void 
AudioManager::AddSoundToEmitter( EmitterHandle& emitter, SoundHandle& sound, const int32 loops, const Real faceTime )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->AddSoundToEmitter( emitter, sound, loops, faceTime );
  }
}

void 
AudioManager::Play( EmitterHandle& handle )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Play( handle, 0.0f );
  }
}


void 
AudioManager::Stop( EmitterHandle& handle )
{
  NE_ASSERT( m_pAudioBackend, "" )();
  
  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Stop( handle, 0.0f );
  }
}


void 
AudioManager::Pause( EmitterHandle& handle )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Pause( handle, 0.0f );
  }
}


void 
AudioManager::Resume( EmitterHandle& handle )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->Resume( handle, 0.0f );
  }
}


Real
AudioManager::GetPlayCursor( EmitterHandle &handle ) const
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    return m_pAudioBackend->GetPlayCursor( handle );
  }
  return 0.0f; 
}


void
AudioManager::SetPlayCursor( EmitterHandle &handle, Real time )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->SetPlayCursor( handle, time );
  }
}


bool
AudioManager::IsPlaying( const EmitterHandle& handle ) const
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    return m_pAudioBackend->IsPlaying( handle );
  }
  return false;
}


bool 
AudioManager::IsPaused( const EmitterHandle& handle ) const
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    return m_pAudioBackend->IsPaused( handle );
  }
  return false;
}


void
AudioManager::SetMasterVolume( float volume )
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    m_pAudioBackend->SetMasterVolume( volume );
  }
}


float 
AudioManager::GetMasterVolume() const
{
  NE_ASSERT( m_pAudioBackend, "" )();

  if( m_pAudioBackend != NULL ) 
  {
    return m_pAudioBackend->GetMasterVolume();
  }
  return 0.0f;
}


