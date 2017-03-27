
#include "AudioDecoder_wav.h"

using namespace Nebulae;


bool isBigEndian()
{
  int a=1;
  return !((char*)&a)[0];
}

int convertToInt(char* buffer,int len)
{
    int a=0;
    if(!isBigEndian())
      for(int i=0;i<len;i++)
        ((char*)&a)[i]=buffer[i];
    else
      for(int i=0;i<len;i++)
        ((char*)&a)[3-i]=buffer[i];    
    return a;
}



WavAudioDecoder::WavAudioDecoder()
{

}


bool
WavAudioDecoder::Parse( File* datastream )
{
  m_stream = datastream;

  char buffer[4];
  m_stream->Read( buffer, 4 );
  if( strncmp( buffer, "RIFF", 4 ) != 0 )
  {
    NE_ASSERT( false, "Expected 'RIFF' in file header" )();
    return false;
  }

  m_stream->Read( buffer, 4 );
  m_stream->Read( buffer, 4 );      //WAVE
  m_stream->Read( buffer, 4 );      //fmt
  m_stream->Read( buffer, 4 );      //16
  m_stream->Read( buffer, 2 );      //1
  m_stream->Read( buffer, 2 );
  m_trackParams.numChannels = convertToInt( buffer, 2 );
  m_stream->Read( buffer, 4 );
  m_trackParams.samplingRate = convertToInt( buffer, 4 );
  m_stream->Read( buffer, 4 );
  /*int byteRate =*/ convertToInt( buffer, 4 );
  m_stream->Read( buffer, 2 );
  m_stream->Read( buffer, 2 );
  m_trackParams.bitsPerSample = convertToInt( buffer, 2 );
  m_stream->Read( buffer, 4 );      //data
  m_stream->Read( buffer, 4 );
	m_trackParams.m_dataSize = convertToInt( buffer, 4 );

  m_dataOffset = m_stream->Tell();

  return true;
}


int32 
WavAudioDecoder::Stream( uint32 start, uint32 size, char* data )
{
  uint64 startPos      = m_dataOffset + start;
	uint64 endPos        = m_dataOffset + m_trackParams.m_dataSize;
	uint64 amountToRead  = size;

	// Bounds checks (and adjustments if possible)
	if( startPos > endPos )
		return 0;

  // Stream file to that point.
  m_stream->Seek( startPos ); 
	std::ifstream::pos_type initial = m_stream->Tell();

  // Check that we don't overflow.
	if( ( startPos + amountToRead ) > endPos )
		amountToRead = endPos - startPos;

  // Attempt to read data.
	m_stream->Read( data, amountToRead );

  // Determine how much was actually read.
  std::ifstream::pos_type final = m_stream->Tell();
  int32 amountRead = final - initial;

  // Success, return how much was read.
  return amountRead;
}
