#ifndef NEBULAE_COMMON_LOG_MODULELOGGER_H
#define NEBULAE_COMMON_LOG_MODULELOGGER_H

namespace Nebulae
{
class Logger;
}

extern "C"
{
  void NE_SetModuleLogger( Nebulae::Logger* logger );
  Nebulae::Logger* NE_GetModuleLogger();
}

#endif // NEBULAE_COMMON_LOG_MODULELOGGER_H
