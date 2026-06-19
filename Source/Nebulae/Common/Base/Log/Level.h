#ifndef NEBULAE_COMMON_BASE_LOG_LEVEL_H_
#define NEBULAE_COMMON_BASE_LOG_LEVEL_H_

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

#endif // NEBULAE_COMMON_BASE_LOG_LEVEL_H_
