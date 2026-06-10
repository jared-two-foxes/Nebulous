
#include <Nebulae/Common/Platform/SharedLibrary.h>

using Nebulae::SharedLibrary;


SharedLibrary::SharedLibrary() = default;


int SharedLibrary::Open( const std::wstring& filename )
{
  handle = LoadLibraryEx( filename.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH );
  if ( handle == nullptr )
  {
    DWORD error = GetLastError();

    return error;
  }

  return 0;
}


void SharedLibrary::Close()
{
  if ( handle != nullptr )
  {
    /* Ignore errors. No good way to signal them without leaking memory. */
    FreeLibrary( handle );
    handle = nullptr;
  }
}


int SharedLibrary::Symbol( const char* name, void** ptr )
{
  *ptr = reinterpret_cast<void*>( GetProcAddress( handle, name ) );

  return ( ( *ptr ) != nullptr ) ? 0 : GetLastError();
}