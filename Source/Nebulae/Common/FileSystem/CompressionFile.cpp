
#include <Nebulae/Common/Common.h>

#include "CompressionFile.h"

#include <zlib.h>


using Nebulae::File;
using Nebulae::CompressionFile;

const unsigned int k_ZlibBufferSize = 4096;


CompressionFile::CompressionFile( File* file )
: File(),
  m_file(file)
{
	// Stream
	std::memset(&m_z_stream, 0, sizeof(m_z_stream));

	// Check
	int ret = inflateInit( &m_z_stream );
	NE_ASSERT( ret == Z_OK, "Failed to aquire zlib buffer" )();
}

CompressionFile::~CompressionFile()
{}

size_t 
CompressionFile::Read( void* buffer, size_t length )
{ 
  std::size_t size;
  std::vector<char > sourceVector( length );

  size = m_file->Read( &sourceVector.front(), length ); //< read in data from file.
  if( size == 0 ) return 0; //< no data in source file, return.

	// Go
  int  ret;
	char outBuffer[k_ZlibBufferSize];
	m_z_stream.next_in  = (Bytef *)(&sourceVector.front());
	m_z_stream.avail_in = static_cast<unsigned int>(size);

  std::vector<char > outVector;

	// inflate the zip file.
	do
	{
		// Get the decompressed bytes blockwise
		m_z_stream.next_out  = reinterpret_cast<Bytef *> (outBuffer);
		m_z_stream.avail_out = sizeof(outBuffer);
		ret = inflate(&m_z_stream, 0);

		// Check
		if( outVector.size() < m_z_stream.total_out )
		{
			// Reserve a few bytes more
			outVector.reserve(m_z_stream.total_out);

			// Append it to the vector
			outVector.insert(outVector.end(), outBuffer, outBuffer + (m_z_stream.total_out - outVector.size()));
		}
	} while( ret == Z_OK );
	
  inflateEnd(&m_z_stream);

	// Check
	NE_ASSERT( ret != Z_STREAM_END, "Exception in zlib stream" )();

	// Recover memory
	outVector.shrink_to_fit();
	
  // Copy to output buffer.
  memcpy( buffer, &outVector.front(), outVector.size() );

  return outVector.size();
}

size_t 
CompressionFile::Write( const void* buffer, size_t length )
{ 
  return m_file->Write( buffer, length );
}
   
void 
CompressionFile::Seek( size_t position )
{
  assert(0);
  m_file->Seek( position );
}
   
void 
CompressionFile::SeekToEnd()
{
  assert(0);
  m_file->SeekToEnd();
}
   
void 
CompressionFile::Skip( size_t bytes )
{
  assert(0);
  m_file->Skip( bytes );
}
   
size_t 
CompressionFile::Tell() const
{ 
  assert(0);
  return m_file->Tell();
}