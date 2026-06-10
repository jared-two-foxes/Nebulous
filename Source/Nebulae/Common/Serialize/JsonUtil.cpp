//
// JsonUtil.cpp
//

#include <Nebulae/Common/Common.h>

bool Nebulae::ParseJSON( File& file, Json::Value* root )
///
///
///
/// @return
///   Whether the parsing was successful.
///
{
  NE_ASSERT( root, "Invalid root object received" );

  Json::Reader reader;
  bool parsingSuccessful = false;
  static int8 staticBuffer[2048];

  file.SeekToEnd();
  std::size_t length = file.Tell();
  file.Seek( 0 );

  if ( length != 0U )
  {
    char* buffer = nullptr;
    if ( length < ARRAYSIZE( staticBuffer ) )
    {
      buffer = staticBuffer;
    }
    else
    {
      buffer = new int8[length];
    }
    if ( length == file.Read( buffer, length ) )
    {
      const char* begin = buffer;
      const char* end = buffer + length;

      parsingSuccessful = reader.parse( begin, end, *root );
    }

    if ( buffer != staticBuffer )
    {
      delete[] buffer;
    }
  }

  NE_ASSERT( parsingSuccessful, "Failed to parse stream with error: %s", reader.getFormattedErrorMessages().c_str() );

  return parsingSuccessful;
}
