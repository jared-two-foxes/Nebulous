#ifndef __NEBULAE_WIN32INCLUDES_H__
#define __NEBULAE_WIN32INCLUDES_H__

#if NE_PLATFORM == NE_PLATFORM_WIN32
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  #define NOMINMAX
  #include <windows.h>
#endif 

#endif // __NEBULAE_WIN32INCLUDES_H__