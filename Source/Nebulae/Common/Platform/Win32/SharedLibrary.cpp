
#include <Nebulae/Common/Platform/SharedLibrary.h>

using Nebulae::SharedLibrary;


SharedLibrary::SharedLibrary()
{}


SharedLibrary::~SharedLibrary()
{}


int 
SharedLibrary::Open( const std::wstring& filename ) 
{
  handle = LoadLibraryEx( filename.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
  if ( handle == nullptr ) 
  {
		DWORD   error = GetLastError();
		HRESULT hr    = HRESULT_FROM_WIN32( error );

    return error;
  }

  return 0;
}


void 
SharedLibrary::Close() 
{
  if ( handle ) 
  {
    /* Ignore errors. No good way to signal them without leaking memory. */
    FreeLibrary( handle );
    handle = nullptr;
  }
}


int 
SharedLibrary::Symbol( const char* name, void** ptr ) 
{
  *ptr = (void*) GetProcAddress( handle, name );

  return *ptr ? 0 : GetLastError();
}