#ifndef NEBULAE_COMMON_LOG_LOGREGISTRY_H
#define NEBULAE_COMMON_LOG_LOGREGISTRY_H

#include <Nebulae/Common/Base/Dependencies.h>
#include <Nebulae/Common/Base/Log/Level.h>


namespace Nebulae
{

class Logger;

class LogRegistry
{
public:
  LogRegistry() = default;
  ~LogRegistry() = default;

  LogRegistry( const LogRegistry& ) = delete;
  LogRegistry& operator=( const LogRegistry& ) = delete;

  Logger& GetOrCreate( const std::string& name, Level defaultMinLevel = Level::Trace );
  Logger* Get( const std::string& name );
  void Register( const std::string& name, std::unique_ptr<Logger> logger );
  void ShutdownAll();

private:
  std::unordered_map<std::string, std::unique_ptr<Logger>> m_loggers;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_LOG_LOGREGISTRY_H
