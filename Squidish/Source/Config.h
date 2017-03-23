//  Nebulae/Squidish/Config.hpp  ---------------------------------------------------//

#ifndef NE_SQUIDISH_CONFIG_H
#define NE_SQUIDISH_CONFIG_H

#include <Nebulae/Common/Config.h>

#ifdef USE_ZIPDEVICE_AS_DEFAULT_ROOT
  #define SQUID_DEFAULT_ROOTDEVICE "zip"
#else 
  #define SQUID_DEFAULT_ROOTDEVICE "disk"
#endif

#endif  // NE_SQUIDISH_CONFIG_H
