
#include <Nebulae/Common/Common.h>

#include "ZipFile.h"

#include <zlib.h>


using Nebulae::File;
using Nebulae::ZipFile;

#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

const unsigned int k_zlibBufferSize = 4096;


ZipFile::ZipFile( /*unzFile stream*/ )
: File(),
  //m_stream( stream ),
  m_pos( 0 )
{
  //int32 err = ExtractFileData( stream, m_buffer );
  //NE_ASSERT( err == UNZ_OK, "Error %d with ZipFile while extracting file data", err )();
  //if( err == UNZ_OK )
  //{
  //  err = unzCloseCurrentFile( m_stream );
  //  NE_ASSERT( err == UNZ_OK, "Error %d with ZipFile in unzCloseCurrentFile\n", err )();
  //}
  //else
  //{
  //  unzCloseCurrentFile( m_stream ); //< don't lose the error
  //}
}

ZipFile::~ZipFile()
{
  
}

size_t 
ZipFile::Read( void* buffer, size_t length )
{ 
  // determine how much of the buffer can actually be copied.
  std::size_t size = std::min<std::size_t>( length, m_buffer.size() - m_pos );

  // copy the data.  
  memcpy( buffer, &m_buffer.front() + m_pos, length );

  // advance the position object.
  m_pos += size;

  // return the length copied.
  return size;
}

size_t 
ZipFile::Write( const void* buffer, size_t length )
{ 
  // Tots not supported right now!
  return 0;
}
   
void 
ZipFile::Seek( size_t position )
{
  m_pos = std::min<std::size_t>( position, m_buffer.size() );
}
   
void 
ZipFile::SeekToEnd()
{
  m_pos = m_buffer.size();
}
   
void 
ZipFile::Skip( size_t bytes )
{
  m_pos = std::min<std::size_t>( m_pos + bytes, m_buffer.size() );
}
   
size_t 
ZipFile::Tell() const
{ 
  return m_pos;
}

//int32
//ZipFile::ExtractFileData( unzFile uf, std::vector<char >& outVector )
//{
//  char  filename_inzip[256];
//  char* filename_withoutpath;
//  char* p;
//  
//  unz_file_info64 file_info;
//  uLong           ratio     = 0;
//  uint32          total_out = 0;
//  int32           err       = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0 );
//  NE_ASSERT( err == UNZ_OK, "Error %d with zipfile in unzGetCurrentFileInfo\n", err)();
//  if( err != UNZ_OK )
//  {
//    return err;
//  }
//
//  p = filename_withoutpath = filename_inzip;
//  while( (*p) != '\0' )
//  {
//    if( ((*p)=='/') || ((*p)=='\\') )
//    {
//      filename_withoutpath = p+1;
//    }
//    p++;
//  }
//
//  err = unzOpenCurrentFile( uf );
//  NE_ASSERT( err == UNZ_OK, "Error %d with ZipFile in unzOpenCurrentFilePassword\n", err)();
// 
//	char outBuffer[k_zlibBufferSize];
//
//  do
//  {
//    err = unzReadCurrentFile( uf, &outBuffer[0], k_zlibBufferSize );
//    NE_ASSERT( err >= 0, "Error %d with zipfile in unzReadCurrentFile\n", err)();
//    if( err < 0 )
//    {
//      break;
//    }
//
//    total_out += static_cast<uint32>( err );
//
//		// Check
//		if( outVector.size() < total_out )
//		{
//			// Reserve a few bytes more
//			outVector.reserve( outVector.size() + err );
//
//			// Append it to the vector
//			outVector.insert( outVector.end(), outBuffer, outBuffer + (total_out - outVector.size()) );
//		}
//  }
//  while( err > 0 );
//
//  return err;
//}