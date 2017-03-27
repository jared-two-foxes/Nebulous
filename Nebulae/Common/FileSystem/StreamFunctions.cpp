
#include <Nebulae/Common/Common.h>

std::size_t
Nebulae::file_read( void *ptr, std::size_t size, std::size_t nmemb, void *datasource )
{
  File& stream = *static_cast<File* >( datasource );

  return stream.Read( ptr, size*nmemb );
}

unsigned 
Nebulae::file_write( void *ptr, unsigned size, unsigned nmemb, void *datasource )
{
  File& stream = *static_cast<File* >( datasource );
    
  return (unsigned)stream.Write( ptr, size*nmemb );
}

int 
Nebulae::file_seek( void *datasource, long offset, int whence )
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
      {
        offset = std::max<long>( 0, offset ); //< cap to an positive number.
        stream.Seek( offset );
      }
      break;

    case SEEK_CUR: // Seek `offset` ahead
      {
        stream.Skip( offset );
      }
      break;

    case SEEK_END: // Seek backwards from the end of the file
      {
        if( offset == 0 )
        {
          stream.SeekToEnd();
        }
        else
        {
          int breakpoint = 12;
        }
      }
      break;

    default:
      return -1;
  }
  
  return (int)stream.Tell();
}

int 
Nebulae::file_close( void *datasource )
{
  return 0;
}

long 
Nebulae::file_tell( void *datasource )
{
  File& stream = *static_cast<File* >( datasource );
  return (long)stream.Tell();
}


/*
unsigned int
ReadProc( void *buffer, unsigned size, unsigned count, fi_handle handle )
{
  std::istream* is = static_cast<std::istream*>(handle);

  std::istream::pos_type initial = is->tellg();  
  is->read( static_cast<std::istream::char_type*>(buffer), size*count );
  return (is->tellg() - initial);
  //return is->readsome( static_cast<std::istream::char_type*>(buffer), size*count );
}

unsigned int
WriteProc( void *buffer, unsigned size, unsigned count, fi_handle handle ) 
{
  std::ostream* os      = static_cast<std::ostream*>(handle);
  std::size_t   initial = os->tellp();
  os->write( (char*)buffer, size*count );
  if( os->bad() ) return 0; // if write fails then tellp is going to fail (return -1)
  return static_cast<uint32>( static_cast<std::size_t>(os->tellp()) - initial );
}

int
SeekProc( fi_handle handle, long offset, int origin ) 
{
  std::istream* is = static_cast<std::istream*>( handle );

  // Determine the seek direction
  std::ios_base::seekdir seekDir;
  switch( origin ) {
    case SEEK_SET: // Seek to absolute position
      seekDir = std::ios::beg;
      break;

    case SEEK_CUR: // Seek `offset` ahead
      seekDir = std::ios::cur;
      break;

    case SEEK_END: // Seek backwards from the end of the file
      seekDir = std::ios::end;
      break;
    
    default: // Unknown seek direction: return failure
      return -1;
  }

  // Seek position in filestream
  is->seekg( offset, seekDir );

  // Check for errors
  if( is->eof() || is->fail() || is->bad() ) {
    return -1;
  }

  // Return the buffer location.
  return is->tellg();
}

long
TellProc( fi_handle handle )
{
  std::istream* is = static_cast<std::istream*>(handle);
  return is->tellg();
}
*/