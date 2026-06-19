#ifndef NEBULAE_COMMON_BASE_LOG_ISINK_H_
#define NEBULAE_COMMON_BASE_LOG_ISINK_H_

#include <Nebulae/Common/Base/Log/LogRecord.h>

namespace Nebulae
{

class ISink
{
public:
  virtual ~ISink() = default;
  virtual void Write( const LogRecord& record ) = 0;
  virtual void Flush() = 0;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_BASE_LOG_ISINK_H_
