//
// JsonUtil.cpp
//

#include <Nebulae/Common/Common.h>

bool
Nebulae::ParseJSON( File& file, Json::Value* root )
///
///
///
/// @return
///   Whether the parsing was successful.
///
{
  NE_ASSERT( root, "Invalid root object received" )(root);

  Json::Reader reader;
  bool         parsingSuccessful = false;
  static int8  static_buffer[2048];
 
  file.SeekToEnd();
  std::size_t length = file.Tell();
  file.Seek(0);

  if( length )
  {
    char* buffer = NULL;
    if( length < ARRAYSIZE(static_buffer) )
    {
      buffer = static_buffer;
    }
    else
    {
      buffer = new int8[length];
    }
    if( length == file.Read(buffer, length) )
    {
      const char* begin = buffer;
      const char* end   = buffer + length;

      parsingSuccessful = reader.parse( begin, end, *root );
    }

    if( buffer != static_buffer )
    {
      delete [] buffer;
    }
  }

  NE_ASSERT( parsingSuccessful, "Failed to parse stream with error: %s", reader.getFormattedErrorMessages().c_str() )();

  return parsingSuccessful;
}
