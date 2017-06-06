//  Nebulae/Common/config.hpp  ---------------------------------------------------//

#ifndef NE_COMMON_CONFIG_H
#define NE_COMMON_CONFIG_H


// Uncomment to use ZipDevice as the default root file access type. 
//#define USE_ZIPDEVICE_AS_DEFAULT_ROOT


#ifdef USE_ZIPDEVICE_AS_DEFAULT_ROOT
  #define NE_DEFAULT_ROOTDEVICE "zip"
#else 
  #define NE_DEFAULT_ROOTDEVICE "disk"
#endif



#endif  // NE_CONFIG_H
