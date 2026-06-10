
#include <Nebulae/Common/Common.h>

#include "CompressionFile.h"

#include <zlib.h>


using Nebulae::CompressionFile;
using Nebulae::File;

const unsigned int K_ZLIB_BUFFER_SIZE = 4096;


CompressionFile::CompressionFile( File* file ) : File(), m_file( file )
{
  // Stream
  std::memset( &m_zStream, 0, sizeof( m_zStream ) );

  // Check
  int ret = inflateInit( &m_zStream );
  NE_ASSERT( ret == Z_OK, "Failed to aquire zlib buffer" );
}

CompressionFile::~CompressionFile() = default;

size_t CompressionFile::Read( void* buffer, size_t length )
{
  std::size_t size;
  std::vector<char> sourceVector( length );

  size = m_file->Read( &sourceVector.front(), length ); //< read in data from file.
  if ( size == 0 )
  {
    return 0; //< no data in source file, return.
  }

  // Go
  int ret;
  char outBuffer[K_ZLIB_BUFFER_SIZE];
  m_zStream.next_in = reinterpret_cast<Bytef*>( &sourceVector.front() );
  m_zStream.avail_in = static_cast<unsigned int>( size );

  std::vector<char> outVector;

  // inflate the zip file.
  do
  {
    // Get the decompressed bytes blockwise
    m_zStream.next_out = reinterpret_cast<Bytef*>( outBuffer );
    m_zStream.avail_out = sizeof( outBuffer );
    ret = inflate( &m_zStream, 0 );

    // Check
    if ( outVector.size() < m_zStream.total_out )
    {
      // Reserve a few bytes more
      outVector.reserve( m_zStream.total_out );

      // Append it to the vector
      outVector.insert( outVector.end(), outBuffer, outBuffer + ( m_zStream.total_out - outVector.size() ) );
    }
  } while ( ret == Z_OK );

  inflateEnd( &m_zStream );

  // Check
  NE_ASSERT( ret != Z_STREAM_END, "Exception in zlib stream" );

  // Recover memory
  outVector.shrink_to_fit();

  // Copy to output buffer.
  memcpy( buffer, &outVector.front(), outVector.size() );

  return outVector.size();
}

size_t CompressionFile::Write( const void* buffer, size_t length ) { return m_file->Write( buffer, length ); }

void CompressionFile::Seek( size_t position )
{
  assert( 0 );
  m_file->Seek( position );
}

void CompressionFile::SeekToEnd()
{
  assert( 0 );
  m_file->SeekToEnd();
}

void CompressionFile::Skip( size_t bytes )
{
  assert( 0 );
  m_file->Skip( bytes );
}

size_t CompressionFile::Tell() const
{
  assert( 0 );
  return m_file->Tell();
}
