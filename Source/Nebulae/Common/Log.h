#ifndef __NEBULAE_COMMON_LOG_H__
#define __NEBULAE_COMMON_LOG_H__

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <fstream>
#include <ctime>

namespace Nebulae {

/// Type alias for log callback function
using LogCallback = void(*)(const char* message);

/// Set the current log callback function
void SetLogCallback(LogCallback cb);

/// Log a formatted message using the current callback or fallback
void Log(const char* format, ...);

/// File logging callback — writes to specified file with timestamp
void FileLogCallback(const char* filename, const char* message);

/// EXE-side file logging callback — writes to nebulous_debug.log with timestamp
void ExeFileCallback(const char* message);

/// DLL-side file logging callback — writes to nebulous_gl_debug.log with timestamp
void DllFileCallback(const char* message);

} // namespace Nebulae

#endif // __NEBULAE_COMMON_LOG_H__
