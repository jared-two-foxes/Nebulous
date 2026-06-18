#include "ConsoleSink.h"

#include <Nebulae/Common/Base/Log/Level.h>

#include <cstdio>

namespace Nebulae
{

ConsoleSink::ConsoleSink( bool useColour ) : m_useColour( useColour ) { (void)m_useColour; }

void ConsoleSink::Write( const LogRecord& record )
{
  const char* level = LevelToString( record.level );
  FILE* stream = ( record.level >= Level::Warning ) ? stderr : stdout;
  std::fprintf( stream, "[%s] [%s] %s\n", level, record.system.c_str(), record.message.c_str() );
}

void ConsoleSink::Flush()
{
  std::fflush( stdout );
  std::fflush( stderr );
}

} // namespace Nebulae
