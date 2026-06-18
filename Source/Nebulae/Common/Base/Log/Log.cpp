#include "Log.h"

namespace
{
std::atomic<Nebulae::Logger*> g_moduleLogger{ nullptr };
} // namespace

extern "C" void NE_SetModuleLogger( Nebulae::Logger* logger )
{
  g_moduleLogger.store( logger, std::memory_order_release );
}

extern "C" Nebulae::Logger* NE_GetModuleLogger() { return g_moduleLogger.load( std::memory_order_acquire ); }

namespace Nebulae
{

namespace detail
{

void DispatchLog( Logger& logger, Level level, const char* system, std::source_location location, std::string message )
{
  LogRecord record;
  record.message = std::move( message );
  record.system = ( system != nullptr ) ? system : "";
  record.level = level;
  record.location = location;
  record.timestamp = std::time( nullptr );
  logger.Enqueue( std::move( record ) );
}

} // namespace detail

} // namespace Nebulae
