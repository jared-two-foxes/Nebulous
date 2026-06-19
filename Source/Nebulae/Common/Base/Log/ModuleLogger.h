#ifndef NEBULAE_COMMON_BASE_LOG_MODULELOGGER_H_
#define NEBULAE_COMMON_BASE_LOG_MODULELOGGER_H_

namespace Nebulae
{
class Logger;
}

extern "C"
{
  void NE_SetModuleLogger( Nebulae::Logger* logger );
  Nebulae::Logger* NE_GetModuleLogger();
}

#endif // NEBULAE_COMMON_BASE_LOG_MODULELOGGER_H_
