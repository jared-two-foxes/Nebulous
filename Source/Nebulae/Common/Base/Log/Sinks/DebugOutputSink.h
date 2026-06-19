#ifndef NEBULAE_COMMON_BASE_LOG_SINKS_DEBUGOUTPUTSINK_H_
#define NEBULAE_COMMON_BASE_LOG_SINKS_DEBUGOUTPUTSINK_H_

#include <Nebulae/Common/Base/Log/ISink.h>

namespace Nebulae
{

class DebugOutputSink : public ISink
{
public:
  void Write( const LogRecord& record ) override;
  void Flush() override;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_BASE_LOG_SINKS_DEBUGOUTPUTSINK_H_
