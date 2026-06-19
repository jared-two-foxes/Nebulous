#ifndef NEBULAE_COMMON_WINDOW_WIN32_WIN32UTILS_H_
#define NEBULAE_COMMON_WINDOW_WIN32_WIN32UTILS_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

namespace Win32Utils
{

/** Converts an error code to a readable string.
 *	The error code this function takes is usually gotten from GetLastError().
 */
std::wstring ErrorCodeToString( DWORD errorCode );

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

}; // namespace Win32Utils

} // namespace Nebulae

#endif // NEBULAE_COMMON_WINDOW_WIN32_WIN32UTILS_H_