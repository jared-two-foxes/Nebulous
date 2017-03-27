
#include "AudioSoundInterface.h"

#include <Nebulae/Audio/AudioDecoder.h>


using namespace Nebulae;


AudioSoundInterface::AudioSoundInterface( File* filehandle ) 
///
/// Default contructor.
///
: m_fileHandle( filehandle ),
  m_decoder( NULL ) 
{
}


AudioSoundInterface::~AudioSoundInterface() 
///
/// Destructor. Needs to free all resources used by this sound.
///
/// @todo [jared.watt 05.05.2013] 
///   Unqueue this sound from the emitter?
///
{
}


const AudioTrackParams& 
AudioSoundInterface::GetTrackParams() const
///
/// Accessor for the basic track data for this sound
///
/// @return
///   The AudioTrackParams structure for this sound.
///
{
  return m_trackParams;
}


void 
AudioSoundInterface::Clear()
///
/// Resets the sound file stream state back to a initial position.
///
/// @return
///   Nothing.
///
{
  if( m_decoder ) {
    m_decoder->Seek( 0.0f );
  }
}


void 
AudioSoundInterface::SetDecoder( AudioDecoder* decoder ) 
///
/// Setter for the object thats used to decode the sound file.  Attempt to parse basic
/// information when set.
///
/// @param decoder
///   The decoder object to store.
///
/// @return
///   Nothing.
///
{
  m_decoder = decoder;
    
  if( m_decoder ) {
    if( m_decoder->Parse( m_fileHandle ) ) {
      SetTrackParams( m_decoder->GetTrackParams() );
    }
  }
}


void
AudioSoundInterface::SetTrackParams( const AudioTrackParams& params )
///
/// Setter for the tracks basic statistics
///
/// @param params
///   The basic statistics of the sound file.
///
/// @return
///   Nothing.
///
{
  m_trackParams.numChannels   = params.numChannels;
  m_trackParams.samplingRate  = params.samplingRate;
  m_trackParams.bitsPerSample = params.bitsPerSample;
  m_trackParams.numSamples    = params.numSamples;
  m_trackParams.m_dataSize    = params.m_dataSize;
}


uint64
AudioSoundInterface::GetDataSize() const
{
  return m_trackParams.m_dataSize;
}


void  
AudioSoundInterface::Seek( float time )
{
  NE_ASSERT( m_decoder, "Attempting to sample a sound that hasn't been properly setup." )();

  if( m_decoder ) {
    m_decoder->Seek( time );
  }
}


double 
AudioSoundInterface::Tell() const
{
  NE_ASSERT( m_decoder, "Attempting to sample a sound that hasn't been properly setup." )();

  if( m_decoder ) {
    return m_decoder->Tell();
  }
  return 0;
}


double 
AudioSoundInterface::GetDuration() const
{
  NE_ASSERT( m_decoder, "Attempting to sample a sound that hasn't been properly setup." )();

  if( m_decoder ) {
    return m_decoder->GetDuration();
  } 
  return 0;
}


int32 
AudioSoundInterface::Sample( uint32 start, uint32 size, char* buffer )
///
/// Attempt to read some data from the sound stream.
///
/// @param start
///   The point to start the streaming from
///
/// @param size
///   The maximum number of bytes to attempt to read
///
/// @param buffer
///   Where to write the streamed data.
///
/// @return 
///   The number of bytes that were sampled
///
{
  NE_ASSERT( m_decoder, "Attempting to sample a sound that hasn't been properly setup." )();

  int32 bytesread = 0;
  if( m_decoder ) {
    bytesread = m_decoder->Stream( start, size, buffer );
  }

  return bytesread;
}