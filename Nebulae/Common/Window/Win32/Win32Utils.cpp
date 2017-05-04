#include "Win32Utils.h"

#include <Nebulae/Common/Window/Win32/Win32Window.h>


namespace Nebulae
{
  
  std::wstring Win32Utils::ErrorCodeToString( DWORD errorCode )
  {
    wchar_t* buffer = nullptr;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorCode,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (wchar_t*)&buffer, 0, nullptr );

    const std::wstring errorMessage( buffer, buffer + wcslen( buffer ) );

    LocalFree( buffer );

    return errorMessage;
  }

  int Win32Utils::MessagePump()
  {
    int iExitCode = 0;

    MSG msg;
    msg.message = WM_NULL;
    
    while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      if( msg.message != WM_QUIT )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
      else
      {
        iExitCode = 1;
      }
    }

    return iExitCode;
  }


  //InitiateOS
  bool Win32Utils::InitiateOS()
  {
    return true;
  }


  //TeardownOS
  bool Win32Utils::TeardownOS()
  {
    //::PostQuitMessage( 0 );
    return true;
  }

}