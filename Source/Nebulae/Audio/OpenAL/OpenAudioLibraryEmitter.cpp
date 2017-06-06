
#include "OpenAudioLibraryEmitter.h"

#include <Nebulae/Audio/AudioSoundInterface.h>
#include <Nebulae/Audio/OpenAL/OpenAudioLibraryBuffer.h>
#include <Nebulae/Audio/OpenAL/OpenAudioLibraryDriver.h>

using namespace Nebulae;


OpenAudioLibraryEmitter::OpenAudioLibraryEmitter( AudioBufferInterface** streamBuffers, int count )
///
/// Constructor.
///
  : AudioEmitterInterface( streamBuffers, count ), 
    m_id( -1 )
{}


OpenAudioLibraryEmitter::~OpenAudioLibraryEmitter()
///
/// Destructor.
///
{
  Destroy();
}


bool
OpenAudioLibraryEmitter::Initiate() 
///
/// Setup of hardware requirements for an emitter object.
///
/// @return
///   Whether the creation of the emitter was successful.
///
{
  alGenSources( 1, &m_id );
  CheckForAudioErrors( "source generation" );

  alSourcef( m_id, AL_PITCH, 1 );
  CheckForAudioErrors( "source pitch" );

  alSourcef( m_id, AL_GAIN, 1 );
  CheckForAudioErrors( "source gain" );

  alSource3f( m_id, AL_POSITION, 0, 0, 0 );
  CheckForAudioErrors( "source position" );

  alSource3f( m_id, AL_VELOCITY, 0, 0, 0 );
  CheckForAudioErrors( "source velocity" );

  alSourcei( m_id, AL_LOOPING, AL_FALSE );
  CheckForAudioErrors( "source looping" );

  return true;
}


void 
OpenAudioLibraryEmitter::Destroy()
///
/// Destroys this instance to be unloaded.  Destroying any internal memory or allocations.
///
/// @return
///   Nothing
///
{
  alDeleteSources( 1, &m_id );

  while( !m_buffers.empty() )
  {
    delete m_buffers.front();
    m_buffers.pop_front();
  }

  while( !m_queued_buffers.empty() )
  {
    delete m_queued_buffers.front();
    m_queued_buffers.pop_front();
  }
}


void
OpenAudioLibraryEmitter::Play()
///
/// Causes the emitter to begin playing whichever sound is currently attached to it.
///
/// @return
///   Nothing.
///
{
  while( !m_buffers.empty() ) {
    StreamBuffer(); 
  }

  alSourcePlay( m_id );
  CheckForAudioErrors( "source play" );
}


void 
OpenAudioLibraryEmitter::Stop()
///
/// Stops the play back of the playing emitter.
///
/// @return
///   Nothing.
///
{
  alSourceStop( m_id );
  CheckForAudioErrors( "source stop" );

  // Unqueue existing buffers
  while( !m_queued_buffers.empty() ) {
    AudioBufferInterface* buffer = m_queued_buffers.front();
    _UnqueueBuffer( buffer );

    m_queued_buffers.pop_front();
    m_buffers.push_back( buffer );
  }

  // Reset the total streamed flag and clear the current status of the audio stream.
  AudioSoundInterface* playingSound = GetPlayingSound();
  if( playingSound != NULL ) {
    playingSound->Clear();
  }
  m_total_buffered = 0;
}


void 
OpenAudioLibraryEmitter::Pause()
///
/// Will stop the playback of Audio from the Emitter.
///
/// @return
///   Nothing.
///
{
  alSourcePause( m_id );
  CheckForAudioErrors( "source pause" );
}


void 
OpenAudioLibraryEmitter::Resume()
///
/// Resumes the sound emitting from the Emitter.
///
/// @return
///   Nothing.
///
{
  while( !m_buffers.empty() ) {
    StreamBuffer(); 
  }

  alSourcePlay( m_id );
  CheckForAudioErrors( "source resume" );
}


bool
OpenAudioLibraryEmitter::IsPlaying() const
///
/// Checks the Emitter status to determine if it is currently playing.
///
/// @return
///   Whether the emitter is currently playing a sound.
///
{
  ALint value;
  alGetSourcei( m_id, AL_SOURCE_STATE, &value );
  return (value == AL_PLAYING);
}


bool 
OpenAudioLibraryEmitter::IsPaused() const
///
/// Checks the Emitter status to determine if it is currently paused.
///
/// @return
///   Whether the emitter is currently playing a sound.
///
{
  ALint value;
  alGetSourcei( m_id, AL_SOURCE_STATE, &value );
  return (value == AL_PAUSED);
}


int 
OpenAudioLibraryEmitter::_BufferProcessed() const
///
/// Queries the AudioSystem for the number of buffers that have completed there playback
/// since last call.
///
/// @return
///   The number of buffers that have finished playing.
///
{
  // Get the processed buffer count
  ALint buffersProcessed = 0;
  alGetSourcei( m_id, AL_BUFFERS_PROCESSED, &buffersProcessed );
  return buffersProcessed;
}


bool 
OpenAudioLibraryEmitter::_EnqueueBuffer( AudioBufferInterface* buffer )
///
/// Will attempt to bind a AudioBuffer to this emitter.  Note that this will cause the buffer
/// to override whatever buffer is currently bound.
///
/// @param buffer
///   The buffer that we which to bind
///
/// @return
///   Whether the binding was successful.
///
{
  OpenAudioLibraryBuffer* openalBuffer = static_cast<OpenAudioLibraryBuffer*>(buffer);

  alSourceQueueBuffers( m_id, 1, &((*openalBuffer).m_id) );
  CheckForAudioErrors("buffer queueing failed");

  //Success
  return true;
}


bool 
OpenAudioLibraryEmitter::_UnqueueBuffer( AudioBufferInterface* buffer )
///
/// Unqueues a Buffer from this emitter. 
///
/// @param buffer
///   The buffer to remove from the playback queue.
///
/// @return
///   Whether the unqueue was successful is not.
///
{
  //@todo [jared.watt] Check if this sound is currently queued on this emitter
  OpenAudioLibraryBuffer* openalBuffer = static_cast<OpenAudioLibraryBuffer*>(buffer);

  alSourceUnqueueBuffers( m_id, 1, &((*openalBuffer).m_id) );
  CheckForAudioErrors("buffer unqueuing failed");

  //Success
  return true;
}
