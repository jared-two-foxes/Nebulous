// Log.cpp

#include <Nebulae/Common/Log.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <fstream>
#include <ctime>

namespace Nebulae {

// Global log callback pointer
static LogCallback g_logCallback = nullptr;

// Mutex for thread-safe file logging
static std::mutex g_logMutex;

/// File logging callback — writes to specified file with timestamp
void FileLogCallback(const char* filename, const char* message)
{
  std::lock_guard<std::mutex> lock(g_logMutex);
  
  std::ofstream log(filename, std::ios::app);
  if (!log.is_open()) {
    return;
  }
  
  // Get current time
  std::time_t now = std::time(nullptr);
  struct tm timeBuf;
  localtime_s(&timeBuf, &now);
  char timeStr[64];
  std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeBuf);
  
  log << "[" << timeStr << "] " << message << std::endl;
  log.close();
}

/// EXE-side file logging callback — writes to nebulous_debug.log with timestamp
void ExeFileCallback(const char* message)
{
  FileLogCallback("nebulous_debug.log", message);
}

/// DLL-side file logging callback — writes to nebulous_gl_debug.log with timestamp
void DllFileCallback(const char* message)
{
  FileLogCallback("nebulous_gl_debug.log", message);
}

/// Set the current log callback function
void SetLogCallback(LogCallback cb)
{
  g_logCallback = cb;
}

/// Log a formatted message using the current callback or fallback
void Log(const char* format, ...)
{
  if (!format) {
    return;
  }
  
  // Format the message
  char buffer[4096];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  
  // Use callback if set, otherwise use EXE-side file fallback
  if (g_logCallback) {
    g_logCallback(buffer);
  } else {
    ExeFileCallback(buffer);
  }
}

} // namespace Nebulae
