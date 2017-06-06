
#include <Nebulae/Common/Common.h>

#include "DiskFile.h"

using namespace Nebulae;

DiskFile::DiskFile( std::string filename, bool bReadOnly )
: File(),
  m_stream(nullptr)
{
  // Always open in binary mode
  // Also, always include reading
  std::ios::openmode mode       = std::ios::in | std::ios::binary;
  std::fstream*      baseStream = 0;

  if( !bReadOnly ) 
  {
    mode |= std::ios::out; //< if data stream is not read only then we should be allowed to right to it. 
  }

  if( !boost::filesystem::exists(filename) ) 
  {
    if( bReadOnly )
    {
      NE_LOG( "Requested path, %s, does not exist!", filename.c_str() );
      return;
    }
  
    mode &= ~std::ios::in; //< remove the "in" as this is not valid if there is no existing file.
  }
      
  baseStream = new std::fstream( filename.c_str(), mode ); 
  
  // Should check ensure open succeeded, in case fail for some reason.
  NE_ASSERT( !baseStream->fail(), "Attempting to open file '%s' failed", filename.c_str() )();
  if( baseStream->fail() )
  {
    delete baseStream;
    baseStream = nullptr;
  }
  else
  {
    m_stream = baseStream;
  }
}

DiskFile::~DiskFile()
{
  if( m_stream )
  {
    m_stream->close();
    delete m_stream; 
  }
}

size_t 
DiskFile::Read( void* buffer, size_t length )
{ 
  m_stream->read( static_cast<std::istream::char_type*>(buffer), length );

  return m_stream->gcount();
}

size_t 
DiskFile::Write( const void* buffer, size_t length )
{ 
  std::size_t initial = m_stream->tellp();
  m_stream->write( (const char*)buffer, length );
  if( m_stream->bad() ) return -1; // if write fails then tellp is going to fail (return -1)
  return static_cast<std::size_t>(m_stream->tellp()) - initial;
}
   
void 
DiskFile::Seek( size_t position )
{
  m_stream->seekg( position, std::ios_base::beg );
}
   
void 
DiskFile::SeekToEnd()
{
  m_stream->seekg( 0, std::ios_base::end );
}
   
void 
DiskFile::Skip( size_t bytes )
{
  m_stream->seekg( bytes, std::ios_base::cur );
}
   
size_t 
DiskFile::Tell() const
{ 
  return m_stream->tellg(); //< input tell.
}