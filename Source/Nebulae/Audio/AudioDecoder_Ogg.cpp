
#include "AudioDecoder_Ogg.h"

using namespace Nebulae;

int
ogg_file_seek( void *datasource, ogg_int64_t offset, int whence )
{
  File& stream = *static_cast<File* >( datasource );

  //// seek fails if the EOF bit is set, so check for it and if it's present
  //// clear the internal state.
  //if( stream.eof() ) {
  //  stream.clear();
  //}
  
  //std::ios_base::seekdir seekDir;
  switch( whence ) {
    case SEEK_SET: // Seek to absolute position
    case SEEK_CUR: // Seek `offset` ahead
    case SEEK_END: // Seek backwards from the end of the file
    
    default:
      return -1;
  }

  stream.Seek( offset/*, seekDir*/ );
  
  return (int)stream.Tell();
}


const ov_callbacks oggVorbis_callbacks =
{
  file_read,
  ogg_file_seek,
  file_close,
  file_tell
};


OggAudioDecoder::OggAudioDecoder()
  : current_section( 0 )
{}

OggAudioDecoder::~OggAudioDecoder()
{
  ov_clear( &vf );
}

void
OggAudioDecoder::Clear()
///
/// Returns the Audio Decoder back to its original state.  ie before any file has been processed.
///
/// @return 
///   Nothing.
///
{
  current_section = 0;
}

bool
OggAudioDecoder::Parse( File* datastream )
///
/// Attempts to setup the meta data of the file and prep the file for streamining.
///
/// @return
///   true  - if filestream is ready to stream
///   false - filestream is not ready to be streamed, libvorbis unable to recognise
///           filetype.
///
{
  int result = ov_open_callbacks( datastream, &vf, NULL, 0, oggVorbis_callbacks );
  if( result < 0 ) {
    fprintf( stderr, "Input does not appear to be an Ogg bitstream.\n" );
    return false;
  }

  //Get track information.
  {
    char** ptr = ov_comment( &vf, -1 )->user_comments;
    vorbis_info* vi = ov_info( &vf, -1 );
    while( *ptr ) {
      fprintf( stderr, "%s\n", *ptr );
      ++ptr;
    }
  
    m_trackParams.numChannels   = vf.vi->channels;        
    m_trackParams.samplingRate  = vf.vi->rate;         
    m_trackParams.bitsPerSample = 16;
    m_trackParams.m_dataSize    = ov_pcm_total( &vf, -1 ) * 4;  // multiplied by 4 because there are 4 bytes per bit?
  }

  return true;
}

bool
OggAudioDecoder::Seek( Real time )
///
/// Seek to a specified time stamp in the sound file.
///
/// @return
///   If the seek was successful.
///
{
  return (ov_time_seek( &vf, time ) == 0);
}

double 
OggAudioDecoder::Tell() const
///
/// Returns the current cursor position into the track in seconds.
///
/// @return
///   the cursor position.
///
{
  return ov_time_tell( const_cast<OggVorbis_File*>(&vf) );
}

double 
OggAudioDecoder::GetDuration() const
///
/// Returns the total duration, in seconds, of the track being decoded.
///
/// @return
///   the total duration.
///
{
  return ov_time_total( const_cast<OggVorbis_File*>(&vf), -1 );  //< -1 to query the entire physical bitstream
}

int32 
OggAudioDecoder::Stream( uint32 start, uint32 size, char* data )
///
/// Attempt to pull /param size bytes from the sound file and store in /param data.
/// 
/// @param start
///   The index into the buffer from which to start streaming from
///
/// @param size
///   The maximum number of bytes to stream.
///
/// @param data
///   The buffer to store streamed data into.
///
/// @return
///   The number of bytes actually streamed.
///
/// @todo [jared.watt 12.07.2013]
///   Seek to the appropriate part of the sound file rather than just sampling the next
///   "size" bytes the file.
///
{
  unsigned long totalread         = 0;
  unsigned int  errorcount        = 0;    
  
  // if we are to start streaming from the beginning then kill the current section.
  if( start == 0 ) {
    current_section = 0;
    ov_pcm_seek( &vf, 0 );
  }
  
  while( totalread < size ) { 
    long int actualread = ov_read( &vf, data + totalread, size-totalread, 0, 2, 1, &current_section );
    if( actualread > 0 ) { // read successful.
      totalread += actualread;  
    } else if( actualread == 0 ) { // EOF
      break;
    } else if( actualread == OV_HOLE || actualread == OV_EBADLINK || actualread == OV_EINVAL ) { // ERROR
      if( ++errorcount >= 3 ) {
				break;
			}
    }
  }

  NE_ASSERT( errorcount == 0, "Encountered an error streaming audio file." )( errorcount );

  return totalread;
}