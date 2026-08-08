#include "LogRegistry.h"

#include <Nebulae/Common/Base/Log/Logger.h>

namespace Nebulae
{

Logger& LogRegistry::GetOrCreate( const std::string& name, Level defaultMinLevel )
{
  auto it = m_loggers.find( name );
  if ( it != m_loggers.end() )
  {
    return *it->second;
  }

  auto logger = std::make_unique<Logger>( name, defaultMinLevel );
  Logger& ref = *logger;
  m_loggers.emplace( name, std::move( logger ) );
  return ref;
}

Logger* LogRegistry::Get( const std::string& name )
{
  auto it = m_loggers.find( name );
  if ( it == m_loggers.end() )
  {
    return nullptr;
  }
  return it->second.get();
}

void LogRegistry::Register( const std::string& name, std::unique_ptr<Logger> logger )
{
  if ( !logger )
  {
    return;
  }
  m_loggers[name] = std::move( logger );
}

void LogRegistry::ShutdownAll()
{
  for ( auto& pair : m_loggers )
  {
    if ( pair.second )
    {
      pair.second->Shutdown();
    }
  }
}

} // namespace Nebulae
