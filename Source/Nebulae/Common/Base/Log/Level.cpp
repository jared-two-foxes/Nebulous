#include "Level.h"

namespace Nebulae
{

const char* LevelToString( Level level )
{
  switch ( level )
  {
  case Level::Trace:
    return "TRACE";
  case Level::Debug:
    return "DEBUG";
  case Level::Info:
    return "INFO";
  case Level::Warning:
    return "WARNING";
  case Level::Error:
    return "ERROR";
  case Level::Critical:
    return "CRITICAL";
  case Level::Off:
    return "OFF";
  default:
    return "UNKNOWN";
  }
}

} // namespace Nebulae
