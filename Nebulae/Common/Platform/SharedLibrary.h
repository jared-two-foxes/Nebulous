// sharedlibrary.h

#ifndef NE_PLATFORM_SHAREDLIBRARY_H
#define NE_PLATFORM_SHAREDLIBRARY_H

#include <Nebulae/Common/Common.h>

namespace Nebulae {

struct SharedLibrary
{
private:
  HMODULE handle;
  char*   errmsg;

  public:
    SharedLibrary();
    ~SharedLibrary();

    // Opens a shared library. The filename is in utf-8. Returns true on success and
    // false on error. Call `SharedLibrary::error()` to get the error message.
    int Open( const std::wstring& path );
    
    void Close();

    // Retrieves a data pointer from a dynamic library. It is legal for a symbol to 
    // map to nullptr. Returns 0 on success and -1 if the symbol was not found.
    int Symbol( const char* name, void** ptr );
    
};

}

#endif