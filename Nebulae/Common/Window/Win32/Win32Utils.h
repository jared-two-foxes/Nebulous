#ifndef __NEBULAE_WINDOWUTILS_H__
#define __NEBULAE_WINDOWUTILS_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Common/Window/Win32/Win32Includes.h>

namespace Nebulae
{

namespace Win32Utils
{

	/** Converts an error code to a readable string.
		*	The error code this function takes is usually gotten from GetLastError().
		*/
	std::string ErrorCodeToString( DWORD errorCode );

	/** Fucntion that cycles all the current systems events that are in the message queue and processes them.
    *  @return.
    *    Returns a non zero value when the system has encountered an error and needs to quit.
		*/
	int MessagePump();

  /** Initiates the OS, Registers basic/default window classes.
    */ 
  bool InitiateOS();

  /** Cleans up os and exits.
    */
  bool TeardownOS();

}; //Win32Utils

} //Nebulae

#endif // __NEBULAE_