#ifndef NEBULAE_COMMON_LOG_LOGRECORD_H
#define NEBULAE_COMMON_LOG_LOGRECORD_H

#include <Nebulae/Common/Base/Dependencies.h>
#include <Nebulae/Common/Base/Log/Level.h>

namespace Nebulae
{

struct LogRecord
{
  std::string message;
  std::string system;
  Level level;
  std::source_location location;
  std::time_t timestamp{ 0 };
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_LOG_LOGRECORD_H
