
#include <Nebulae/Audio/AudioDecoder.h>
#include <Nebulae/Audio/AudioDecoder_wav.h>
#include <Nebulae/Audio/AudioDecoder_Ogg.h>
#include <Nebulae/Audio/AudioEmitterInterface.h>
#include <Nebulae/Audio/AudioSoundInterface.h>
#include <Nebulae/Audio/EmitterHandle.h>
#include <Nebulae/Audio/SoundHandle.h>

#include <Nebulae/Audio/OpenAL/OpenAudioLibraryDriver.h>

#include "AudioBackend.h"

using namespace Nebulae;

bool CheckForAudioErrors( const char* message )
{
  std::string errorMessage; 
  ALCenum error = alGetError();
  switch( error )
  {
    case AL_INVALID_NAME:
      errorMessage = "Invalid Name";
      break;
    case AL_INVALID_ENUM:
      errorMessage = "Invalid Enum";
      break;

    case AL_INVALID_VALUE:
      errorMessage = "Invalid Value";
      break;

    case AL_INVALID_OPERATION:
      errorMessage = "Invalid Operation";
      break;

    case AL_OUT_OF_MEMORY:
      errorMessage = "Out of Memory";
      break;

    case AL_NO_ERROR:
    default:
      break;
  }
   
  NE_ASSERT( error == AL_NO_ERROR, "OpenAL error 0x%x (%u) detected", error, error )( errorMessage.c_str() );

  return error == AL_NO_ERROR;
}

AudioBackend::AudioBackend( FileSystemPtr fileSystem )
///
/// Default Constructor
///
  : m_fileSystem( fileSystem ),
    m_pDriver( 0 ),
    m_volume( 1.0f )
{
}


AudioBackend::~AudioBackend()
///
/// Deconstructor.
///
{
  Destroy();
}


bool 
AudioBackend::Initiate()
///
/// Setup function for the Audio subsystem.
///
/// @return
///   Whether initiation was successful.
///
{
  // Create the driver object.
  m_pDriver = new OpenAudioLibraryDriver();
  
  // Attempt to initialize the driver
  bool success = m_pDriver->Init();
  if( success )
  {
    //Attempt to grab/set the master volume.
    m_pDriver->SetMasterVolume( 1.0f );
  }
  
  return success;
}


void 
AudioBackend::Destroy()
///
/// Deallocates all the current resources and destroys the driver object.
///
/// @return 
///   Nothing.
///
{
  // Delete all the current remaining emitters.
  std::for_each( m_emitters.begin(), m_emitters.end(), [](AudioEmitterInterface* emitter){
    delete emitter;
  });
  m_emitters.clear();
  
  // Delete all the current remaining sounds.
  std::for_each( m_sounds.begin(), m_sounds.end(), [](AudioSoundInterface* sound){
    delete sound;
  });
  m_sounds.clear();

  // Destroy the driver object.
  if( m_pDriver != NULL )
  {
    delete m_pDriver;
  }
  m_pDriver = 0;
}


void 
AudioBackend::Update( const Real elapsed )
///
/// Updates all sound emitters.
///
/// @param elapsed
///   The time step between calls.
///
/// @return
///   Nothing.
///
{
  // Update emitters
  std::vector<AudioEmitterInterface*>::iterator end_it = m_emitters.end();
  for( std::vector<AudioEmitterInterface*>::iterator it = m_emitters.begin(); it != end_it; ++it ) 
  {
    //@todo [jared.watt]  Add in something here to update only those emitters that are currently playing?
    (*it)->Update( elapsed );
  }
}


SoundHandle 
AudioBackend::CreateSound( const char* filename )
///
/// Creates a Sound object based upon the /param filename
///
/// @param filename
///   The path to the audio file that the sound is to represent.
///
/// @return
///   Reference to the Sound object used to identify this audio file.
///
/// @todo [jared.watt 09.06.2013]
///   Need to determine the correct decoder to attach based upon file type, fallback on extension?
///
{
  NE_ASSERT( m_pDriver != NULL, "Sound driver is uninitialized." )();
  NE_ASSERT( m_fileSystem != NULL, "FileSystem is uninitialized." )();

  if( m_pDriver != NULL ) 
  {
    char filenameBuffer[128];
    sprintf( filenameBuffer, "sounds/%s", filename );

    File* datastream = m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filenameBuffer );
    if( datastream == NULL ) 
    {
      return SoundHandle();
    }

    // Create the decoder
    AudioDecoder* decoder   = NULL;
    std::string   extension = boost::filesystem::extension( filename );
    if( extension == ".wav" )
    {
       decoder = new WavAudioDecoder();
    }
    else if( extension == ".ogg" )
    {
      decoder = new OggAudioDecoder();
    }
    else
    {
      NE_ASSERT( false, "Unknown decoder type found." )( extension );
    }

    // Create sound data structure & initialize it.
    AudioSoundInterface* sound = new AudioSoundInterface( datastream );
    sound->SetDecoder( decoder );
    
    // Store the structure
    m_sounds.push_back( sound );
    
    // Create the handle & return.
    return SoundHandle( m_sounds.size(), this, sound );
  }

  //Return Null Handle.
  return SoundHandle();
}


Real
AudioBackend::GetDuration( SoundHandle handle ) const
{
  return handle.m_pInternal->GetDuration();
}


EmitterHandle 
AudioBackend::CreateEmitter()
///
/// Creates an Emitter based upon a sound handle.
///
/// @param sound
///   A sound to play with the emitter
///
/// @return
///   Handle to an Emitter.
///
/// @todo [jared.watt 10.06.2013]
///   Remove the dependency of the SoundHandle.  It makes no sense
///   for an emitter to only play one sound.  Function should instead
///   take a position of where the emitter is.
///
{
  NE_ASSERT( m_pDriver, "Sound driver is uninitialized." )();

  if( m_pDriver != NULL ) 
  {
    AudioEmitterInterface* emitter = m_pDriver->CreateDriverEmitter();
    if( emitter != NULL ) 
    {
      m_emitters.push_back( emitter );
      std::size_t id = m_emitters.size();
      return EmitterHandle(id, this, emitter);
    }
  }
  
  return EmitterHandle();
}


void 
AudioBackend::AddSoundToEmitter( EmitterHandle& emitter, SoundHandle& sound, const int32 loops, const Real fadeTime )
///
/// Will push a sound to the specified emitter.
///
/// @return
///   Nothing.
///
{
  emitter.m_pInternal->Push( sound.m_pInternal, loops/*, fadeTime*/ );
}


SoundHandle 
AudioBackend::GetCurrentSound( EmitterHandle& handle )
{
  AudioSoundInterface* sound = handle.m_pInternal->GetPlayingSound();
  if( sound != NULL ) 
  {
    return SoundHandle( m_sounds.size(), this, sound );
  }

  // return NULL Handle.
  return SoundHandle();
}


void 
AudioBackend::Play( EmitterHandle& handle, const Real fadeTime )
///
/// Cause the emitter to begin emitting whatever sounds are currently bound to it
///
/// @param handle
///   The emitter to play from
///
/// @param loop [TODO]
///   Should the sounds repeat.
///
/// @param fadeTime [TODO]
///   The amount of time that the sound should take to fade in.
///
/// @return
///   Nothing.
///
{
  handle.m_pInternal->Play();
}


void 
AudioBackend::Stop( EmitterHandle& handle, const Real fadeTime )
///
/// Stop an emitter from playing.
///
/// @param handle
///   The emitter to stop emitting from.
///
/// @param fadeTime [TODO]
///   The amount of time that the sound should take to fade out.
///
/// @return
///   Nothing.
///
{
  handle.m_pInternal->Stop();
}


void 
AudioBackend::Pause( EmitterHandle& handle, const Real fadeTime )
///
/// Pause the playback from an emitter.
///
/// @param handle
///   The emitter to pause emitting from.
///
/// @param fadeTime [TODO]
///   The amount of time that the sound should take to fade out.
///
/// @return
///   Nothing.
///
{
  handle.m_pInternal->Pause();
}


void 
AudioBackend::Resume( EmitterHandle& handle, const Real fadeTime )
///
/// Resume the playback from an emitter.
///
/// @param handle
///   The emitter to resume emitting from.
///
/// @param fadeTime [TODO]
///   The amount of time that the sound should take to fade out.
///
/// @return
///   Nothing.
///
{
  handle.m_pInternal->Resume();
}


Real  
AudioBackend::GetPlayCursor( EmitterHandle& handle ) const
{
  return (Real)handle.m_pInternal->GetPlayCursor();
}


void
AudioBackend::SetPlayCursor( EmitterHandle& handle, Real time )
{
  handle.m_pInternal->SetPlayCursor( time );
}


bool 
AudioBackend::IsPlaying( const EmitterHandle& handle ) const
///
/// Checks if an Audio Emitter is currently playing a sound.
///
/// @param handle
///   The audio emitter to check.
///
/// @return
///   Whether the emitter is playing a sound.
///
{
	return handle.m_pInternal->IsPlaying();
}


bool
AudioBackend::IsPaused( const EmitterHandle& handle ) const
///
/// Checks if an AudioEmitter is currently paused.
///
/// @param handle
///   The audio emitter to check.
///
/// @return
///   Whether the emitter is paused.
///
{
	return handle.m_pInternal->IsPaused();
}


void 
AudioBackend::SetMasterVolume( float volume )
///
/// Setter function for setting the maximum sound volume for all of the 
/// sounds in the subsystem.
///
/// @param volume
///   The max volume to use.
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_pDriver, "Sound driver is uninitialized." )();

  if( m_pDriver != NULL )
  {
    // Store the current volume for retrieval.
    m_volume = volume;
    // Set volume.
    m_pDriver->SetMasterVolume( m_volume );
  }
}


float 
AudioBackend::GetMasterVolume() const
///
/// Accessor for the current overall volume of the sound subsystem.
///
/// @return
///   The max volume for all the current sound groups.
///
{
  return m_volume;
}
