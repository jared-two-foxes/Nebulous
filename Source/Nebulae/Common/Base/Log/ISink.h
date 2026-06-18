#ifndef COMMON_LOG_ISINK_H__
#define COMMON_LOG_ISINK_H__

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

#endif // COMMON_LOG_ISINK_H__
