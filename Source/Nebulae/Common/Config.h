//  Nebulae/Common/config.hpp  ---------------------------------------------------//

#ifndef NEBULAE_SOURCE_NEBULAE_COMMON_CONFIG_H_
#define NEBULAE_SOURCE_NEBULAE_COMMON_CONFIG_H_


// Uncomment to use ZipDevice as the default root file access type.
// #define USE_ZIPDEVICE_AS_DEFAULT_ROOT


#ifdef USE_ZIPDEVICE_AS_DEFAULT_ROOT
#define NE_DEFAULT_ROOTDEVICE "zip"
#else
#define NE_DEFAULT_ROOTDEVICE "disk"
#endif


#endif // NEBULAE_SOURCE_NEBULAE_COMMON_CONFIG_H_
