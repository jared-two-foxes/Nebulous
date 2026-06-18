#ifndef COMMON_LOG_LEVEL_H__
#define COMMON_LOG_LEVEL_H__

#include <cstdint>

namespace Nebulae
{

enum class Level : uint8_t
{
  Trace = 0,
  Debug,
  Info,
  Warning,
  Error,
  Critical,
  Off
};

const char* LevelToString( Level level );

} // namespace Nebulae

#endif // COMMON_LOG_LEVEL_H__
