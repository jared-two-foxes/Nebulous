#ifndef NEBULAE_COMMON_COMMANDLINE_H
#define NEBULAE_COMMON_COMMANDLINE_H

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class CommandLine
{
private:
  std::map<std::string, bool > m_flags;
  std::map<std::string, std::string > m_options;

public:  
  CommandLine();

  uint32 Init( int argc, char* argv[] );

  void AddOption( const char* option, const char* default_value );
  
  void AddFlag( const char* flag );

  bool IsSet( const char* flag );

};

}

#endif // NEBULAE_COMMON_COMMANDLINE_H