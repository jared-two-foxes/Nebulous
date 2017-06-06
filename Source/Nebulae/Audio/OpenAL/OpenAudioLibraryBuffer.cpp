
#include "OpenAudioLibraryBuffer.h"

#include <Nebulae/Audio/AudioDecoder.h>
#include <Nebulae/Audio/OpenAL/OpenAudioLibraryDriver.h>

namespace Nebulae
{
  ///
  /// Default Constructor
  ///
  OpenAudioLibraryBuffer::OpenAudioLibraryBuffer( uint32 size )
    : m_id(0) 
  {
  
  }

  OpenAudioLibraryBuffer::~OpenAudioLibraryBuffer() 
  ///
  /// Destructor
  ///
  {
    Destroy();
  }

  bool
  OpenAudioLibraryBuffer::Initiate() 
  ///
  /// Generates the OpenAL buffer.
  ///
  /// @return
  ///   Was the generation of the buffer successful.
  ///
  {
    //Generate the buffer
    alGenBuffers( 1, &m_id );
    CheckForAudioErrors("buffer generation");

    //Success
    return true;
  }
  
  bool
  OpenAudioLibraryBuffer::Fill( AudioFormat format, void* data, uint32 size, uint32 frequency )
  ///
  /// Copys contents of data into internal buffer.
  ///
  /// @param data
  ///   The data store to copy.
  ///
  /// @param size
  ///   The amount of data to copy.
  ///
  /// @returns 
  ///   If the buffer copy was successful.
  ///
  /// @todo [jared.watt 09.06.2013]  
  ///   Should check that the buffer size isn't bigger than the created buffer?
  ///
  {
    // Convert the audio format to an OpenAL format?
    ALenum convertedFormat = AL_FORMAT_MONO8; // format == AUDIO_FORMAT_MONO8
    if( format == AUDIO_FORMAT_MONO16 )
      convertedFormat = AL_FORMAT_MONO16;
    else if( format == AUDIO_FORMAT_STEREO8 )
      convertedFormat = AL_FORMAT_STEREO8;
    else if( format == AUDIO_FORMAT_STEREO16 )
      convertedFormat = AL_FORMAT_STEREO16;

    //Store parsed data in hardware for playback.
    alBufferData( m_id, convertedFormat, data, size, frequency );
    CheckForAudioErrors("buffer copy");

    //Success
    return true;
  }

  bool
  OpenAudioLibraryBuffer::Destroy() 
  ///
  /// Flags with OpenAL to destroy this buffer.  We can now forget about it, OpenAL will clean this up
  /// for us.
  ///
  /// @return
  ///   If the buffer deletion succeeded.
  ///
  {
    //Destroy the buffer.
    alDeleteBuffers( 1, &m_id );
    CheckForAudioErrors("buffer destruction");

    //Success
    return true;
  }
}