// sharedlibrary.h

#ifndef NEBULAE_COMMON_PLATFORM_SHAREDLIBRARY_H_
#define NEBULAE_COMMON_PLATFORM_SHAREDLIBRARY_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

struct SharedLibrary
{
private:
  HMODULE handle;
  char* errmsg;

public:
  SharedLibrary();
  ~SharedLibrary() = default;

  // Opens a shared library. The filename is in utf-8. Returns true on success and
  // false on error. Call `SharedLibrary::error()` to get the error message.
  int Open( const std::wstring& path );

  void Close();

  // Retrieves a data pointer from a dynamic library. It is legal for a symbol to
  // map to nullptr. Returns 0 on success and -1 if the symbol was not found.
  int Symbol( const char* name, void** ptr );
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_PLATFORM_SHAREDLIBRARY_H_