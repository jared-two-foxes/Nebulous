
#include "AudioEmitterInterface.h"

#include "Nebulae/Audio/AudioBufferInterface.h"
#include "Nebulae/Audio/AudioSoundInterface.h"

using namespace Nebulae;

static const int BUFFER_SIZE = 4096 * 8;

AudioEmitterInterface::AudioEmitterInterface( AudioBufferInterface** streamBuffers, int count )
///
/// Default constructor
///
  : m_total_buffered( 0 )
{
  while( count > 0 ) {
    m_buffers.push_back( streamBuffers[--count] );
  }
}


AudioEmitterInterface::~AudioEmitterInterface()
///
/// Desstructor
///
{}


AudioSoundInterface* 
AudioEmitterInterface::GetPlayingSound() const
{
  return m_queuedSounds.empty() ? nullptr : m_queuedSounds.front().sound;
}


void 
AudioEmitterInterface::Update( const Real elapsed )
///
/// Per frame update function.  Checks current status, if current sound has finished playing 
/// will check the queue for a new sound to start playing.
///
/// @return
///   Nothing.
///
{
//
// Check if current sound stream needs updating.  This should be everytime that a new buffer starts playing.
//
  uint32 buffersStreamed = _BufferProcessed();
  bool   queuedEmptied   =  buffersStreamed > 0 && buffersStreamed >= m_queued_buffers.size();
  if( buffersStreamed > 0 ) {
    // Unqueue some buffers?
    NE_ASSERT( m_queued_buffers.size() >= buffersStreamed, "" )( buffersStreamed );
    while( buffersStreamed > 0 ) {
      AudioBufferInterface* buffer = m_queued_buffers.front();
      _UnqueueBuffer( buffer );

      m_queued_buffers.pop_front();

      m_buffers.push_back( buffer );

      buffersStreamed--;
    }
  }

  //@todo check if there are any remaining queued buffers while we have no sound and unqueue them?

  AudioSoundInterface* sound = GetPlayingSound();
  if( !m_buffers.empty() && nullptr != sound ) {
    StreamBuffer();
  }
  
  // Check if we managed to fill some more buffers even thou we emptied the buffers previously.
  if( queuedEmptied && !m_queued_buffers.empty() ) {
    Resume();
  }
}



Real 
AudioEmitterInterface::GetPlayCursor() const
///
/// Retrieves the current progress of the emitter through the currently bound playing sound.
///
/// @return
///   The current cursor position.
///
{ 
  AudioSoundInterface* playingSound = GetPlayingSound();
  if( playingSound != NULL ) {
    return playingSound->Tell();
  }

  return 0; 
}


void 
AudioEmitterInterface::SetPlayCursor( Real time )
///
/// Sets the emitter progress through the currently bound sound, if it exists, to the @param
/// time to continue playback from 
///
/// @param time
///   The value to progress to in the current sound.
///
///
/// @return
///   Nothing.
///
{ 
  AudioSoundInterface* playingSound = GetPlayingSound();
  if( playingSound ) {
    playingSound->Seek( time ); 
  }
}


void
AudioEmitterInterface::Push( AudioSoundInterface* sound, int32 loops, bool force )
///
/// Adds a sound to this emitters sound queue for playing.
///
/// @param sound
///   The sound that we are adding to the sounds queue.
///
/// @param force
///   Forces sound playback to begin immediately.
///
/// @return
///   Nothing.
///
{
  QueuedEmitterSound queuedSound;
  queuedSound.sound = sound;
  queuedSound.loops = loops;

  // if we are forcing this sound pop off all of the existing sounds.
  while( force && !m_queuedSounds.empty() ) {
    Pop();
  }

  // add sound to the queue.
  m_queuedSounds.push_back( queuedSound );
  
  // if we are to force this sound start streaming it now.
  if( force ) {
    m_total_buffered = 0;
    if( !m_buffers.empty() ) {
      StreamBuffer();
    }
  }
}


void 
AudioEmitterInterface::Pop()
///
/// Removes the sound at the front of the sound queue and pops any
/// currently queued buffer streams. 
///
/// @return
///   Nothing.
///
{
  // Remove the top sound file.
  if( m_queuedSounds.empty() ) {
    m_queuedSounds.pop_front();
    m_total_buffered = 0;
  }

  // Unbind all of the bound audio buffers.
  while( !m_queued_buffers.empty() ) {
    AudioBufferInterface* buffer = m_queued_buffers.back();
    _UnqueueBuffer( buffer );
    m_queued_buffers.pop_back();
    m_buffers.push_back( buffer );
  }
}

bool 
AudioEmitterInterface::StreamBuffer()
///
/// Will stream a segment of the currently playing sound into the next sound buffer.
///
/// @return
///   Nothing.
///
{
  if( m_buffers.empty() ) {
    //@todo [jared.watt]  What to do if we don't have any buffers left?
    return false;
  }
  
  // Cannot continue streaming if there is no sound.
  if( m_queuedSounds.empty() ) {
    return false;
  }

  QueuedEmitterSound& queuedSound = m_queuedSounds.front();
  NE_ASSERT( queuedSound.sound != NULL, "" )();

  char data[ BUFFER_SIZE ];
  memset( data, 0, BUFFER_SIZE );
  int bytes = queuedSound.sound->Sample( m_total_buffered, BUFFER_SIZE, data );
  NE_ASSERT( bytes >= 0, "Failed to sample data from sound." )( bytes, m_total_buffered );
  if( bytes == -1 ) {
    return false; //Something bad happened
  } else if( bytes == 0 ) {
    if( queuedSound.loops > 0 || queuedSound.loops == -1 ) {
      // We have reached the end of the sound file, loop it.
      m_total_buffered = 0;
      if( queuedSound.loops != -1 ) {
        queuedSound.loops--;
      }
    } else {
      // Theres nothing left to stream so pop the current sound from the front and 
      // leave the remaining queued buffers to be played play out.
      m_queuedSounds.pop_front();
      m_total_buffered = 0; //< reset the buffered count.
    }
  } else if( bytes > 0 ) {
    // Then load that buffer into one of the Emitters buffer objects.
    const AudioTrackParams& params    = queuedSound.sound->GetTrackParams();
    uint32                  frequency = params.samplingRate;
    AudioFormat             format    = AUDIO_FORMAT_MONO8;

    if( params.numChannels == 1 && params.bitsPerSample == 8 ) {
      format = AUDIO_FORMAT_MONO8;
    } else if( params.numChannels == 1 && params.bitsPerSample == 16 ) {
      format = AUDIO_FORMAT_MONO16;
    } else if( params.numChannels == 2 && params.bitsPerSample == 8 ) {
      format = AUDIO_FORMAT_STEREO8;
    } else if( params.numChannels == 2 && params.bitsPerSample == 16 ) {
      format = AUDIO_FORMAT_STEREO16;
    } else {
      NE_ASSERT( false, "Channel cound and bitsPerSample are not compatable" )( params.numChannels, params.bitsPerSample );
      return false;
    }

    // Grab the next buffer
    AudioBufferInterface* buffer = m_buffers.front(); 
    // Remove it from the front of the queue.
    m_buffers.pop_front();

    // Fill it with required data.
    if( buffer->Fill(format,data,bytes,frequency) ) {
      _EnqueueBuffer( buffer );
      m_queued_buffers.push_back( buffer );
      m_total_buffered += bytes;
    } else { //Shit. Something went wrong binding that data to the buffer?
      m_buffers.push_back( buffer ); // Push buffer back onto the buffer list.
      NE_ASSERT( false, "Failed to fill the audio buffer with data." )();
      return false;
    }
  }

  return true;
}