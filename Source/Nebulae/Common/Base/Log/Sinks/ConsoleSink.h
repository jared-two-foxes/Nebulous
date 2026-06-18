#ifndef NEBULAE_COMMON_LOG_SINKS_CONSOLESINK_H
#define NEBULAE_COMMON_LOG_SINKS_CONSOLESINK_H

#include <Nebulae/Common/Base/Log/ISink.h>

namespace Nebulae
{

class ConsoleSink : public ISink
{
public:
  explicit ConsoleSink( bool useColour = true );
  void Write( const LogRecord& record ) override;
  void Flush() override;

private:
  bool m_useColour;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_LOG_SINKS_CONSOLESINK_H
