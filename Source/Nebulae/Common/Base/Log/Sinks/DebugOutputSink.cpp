#include "DebugOutputSink.h"

#include <Nebulae/Common/Base/Log/Level.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>

namespace Nebulae
{

void DebugOutputSink::Write( const LogRecord& record )
{
#ifdef _WIN32
  std::string line =
    std::string( "[" ) + LevelToString( record.level ) + "] [" + record.system + "] " + record.message + "\n";
  OutputDebugStringA( line.c_str() );
#else
  (void)record;
#endif
}

void DebugOutputSink::Flush() {}

} // namespace Nebulae
