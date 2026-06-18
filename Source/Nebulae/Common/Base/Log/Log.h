#ifndef COMMON_LOG_LOG_H__
#define COMMON_LOG_LOG_H__

#include <Nebulae/Common/Base/Dependencies.h>

#include <Nebulae/Common/Base/Log/Level.h>
#include <Nebulae/Common/Base/Log/LogRecord.h>
#include <Nebulae/Common/Base/Log/Logger.h>
#include <Nebulae/Common/Base/Log/ModuleLogger.h>

namespace Nebulae
{

namespace detail
{

template <typename T> std::string ToString( T&& value )
{
  std::ostringstream stream;
  stream << std::forward<T>( value );
  return stream.str();
}

inline std::string Format( const char* format ) { return format ? std::string( format ) : std::string(); }

template <typename... Args> std::string Format( const char* format, Args&&... args )
{
  std::string result = format ? std::string( format ) : std::string();
  const std::string values[] = { ToString( std::forward<Args>( args ) )... };

  for ( const std::string& value : values )
  {
    const size_t pos = result.find( "{}" );
    if ( pos == std::string::npos )
      break;
    result.replace( pos, 2, value );
  }

  return result;
}

inline std::source_location CurrentSourceLocation() { return std::source_location::current(); }

void DispatchLog( Logger& logger, Level level, const char* system, std::source_location location, std::string message );

} // namespace detail

} // namespace Nebulae

#define NE_LOG_TO_IMPL_( logger, level, system, fmt, ... )                                                \
  do                                                                                                      \
  {                                                                                                       \
    if ( ( level ) >= ( logger ).GetMinLevel() )                                                          \
    {                                                                                                     \
      ::Nebulae::detail::DispatchLog( ( logger ), ( level ), ( system ), std::source_location::current(), \
                                      ::Nebulae::detail::Format( ( fmt ), ##__VA_ARGS__ ) );              \
    }                                                                                                     \
  } while ( 0 )

#define NE_LOG_IMPL_( level, system, fmt, ... )                                                            \
  do                                                                                                       \
  {                                                                                                        \
    ::Nebulae::Logger* _ne_logger = NE_GetModuleLogger();                                                  \
    if ( _ne_logger != nullptr && ( level ) >= _ne_logger->GetMinLevel() )                                 \
    {                                                                                                      \
      ::Nebulae::detail::DispatchLog( *_ne_logger, ( level ), ( system ), std::source_location::current(), \
                                      ::Nebulae::detail::Format( ( fmt ), ##__VA_ARGS__ ) );               \
    }                                                                                                      \
  } while ( 0 )

#define NE_LOG_TRACE_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Trace, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_DEBUG_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Debug, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_INFO_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Info, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_WARN_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Warning, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_ERROR_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Error, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_CRITICAL_TO( logger, system, fmt, ... ) \
  NE_LOG_TO_IMPL_( logger, ::Nebulae::Level::Critical, system, fmt, ##__VA_ARGS__ )

#define NE_LOG_TRACE( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Trace, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_DEBUG( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Debug, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_INFO( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Info, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_WARN( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Warning, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_ERROR( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Error, system, fmt, ##__VA_ARGS__ )
#define NE_LOG_CRITICAL( system, fmt, ... ) NE_LOG_IMPL_( ::Nebulae::Level::Critical, system, fmt, ##__VA_ARGS__ )

#endif // __NEBULAE_COMMON_LOG_H__
