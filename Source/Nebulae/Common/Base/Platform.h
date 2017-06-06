#ifndef __NEBULAE_PLATFORM_H__
#define __NEBULAE_PLATFORM_H__


//----------------------------------------------------------------------------
//  Initial platform/compiler-related stuff to set.
#define NE_PLATFORM_WIN32     1
#define NE_PLATFORM_LINUX     2
#define NE_PLATFORM_APPLE     3
#define NE_PLATFORM_APPLE_IOS 4
#define NE_PLATFORM_ANDROID   5

#define NE_COMPILER_MSVC      1
#define NE_COMPILER_GNUC      2
#define NE_COMPILER_BORL      3
#define NE_COMPILER_WINSCW    4
#define NE_COMPILER_GCCE      5
#define NE_COMPILER_CLANG     6

#define NE_ENDIAN_LITTLE      1
#define NE_ENDIAN_BIG         2

#define NE_ARCHITECTURE_32    1
#define NE_ARCHITECTURE_64    2

//----------------------------------------------------------------------------
// Finds the compiler type and version. 
#if defined( __GCCE__ )
  #define NE_COMPILER NE_COMPILER_GCCE
  #define NE_COMP_VER _MSC_VER
#elif defined( __WINSCW__ )
  #define NE_COMPILER NE_COMPILER_WINSCW
  #define NE_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
  #define NE_COMPILER NE_COMPILER_MSVC
  #define NE_COMP_VER _MSC_VER
#elif defined( __clang__ )
  #define NE_COMPILER NE_COMPILER_CLANG
  #define NE_COMP_VER (((__clang_major__)*100) + \
        (__clang_minor__*10) + \
        __clang_patchlevel__)
#elif defined( __GNUC__ )
  #define NE_COMPILER NE_COMPILER_GNUC
  #define NE_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
  #define NE_COMPILER NE_COMPILER_BORL
  #define NE_COMP_VER __BCPLUSPLUS__
  #define __FUNCTION__ __FUNC__ 
#else
  #pragma error "No known compiler. Abort! Abort!"
#endif


//----------------------------------------------------------------------------
// See if we can use __forceinline or if we need to use __inline instead. 
#if NE_COMPILER == NE_COMPILER_MSVC
  #if NE_COMP_VER >= 1200
    #define FORCEINLINE __forceinline
  #endif
#elif defined(__MINGW32__)
  #if !defined(FORCEINLINE)
    #define FORCEINLINE __inline
  #endif
#else
  #define FORCEINLINE __inline
#endif

  
//----------------------------------------------------------------------------
// Finds the current platform. 
#if defined( __WIN32__ ) || defined( _WIN32 )
  #define NE_PLATFORM NE_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
    // Device                                                     Simulator
    // Both requiring OS version 4.0 or greater
  #if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
    #define NE_PLATFORM NE_PLATFORM_APPLE_IOS
  #else
    #define NE_PLATFORM NE_PLATFORM_APPLE
  #endif
#elif defined(__ANDROID__)
  #define NE_PLATFORM NE_PLATFORM_ANDROID
#else
  #define NE_PLATFORM NE_PLATFORM_LINUX
#endif


//----------------------------------------------------------------------------
// Find the arch type 
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
  #define NE_ARCH_TYPE NE_ARCHITECTURE_64
#else
  #define NE_ARCH_TYPE NE_ARCHITECTURE_32
#endif


//----------------------------------------------------------------------------
// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define NE_QUOTE_INPLACE(x) # x
#define NE_QUOTE(x) NE_QUOTE_INPLACE(x)
#define NE_WARN(x)  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )


//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set NE_ENDIAN correctly
#ifdef NE_CONFIG_BIG_ENDIAN
  #define NE_ENDIAN NE_ENDIAN_BIG
#else
  #define NE_ENDIAN NE_ENDIAN_LITTLE
#endif

//----------------------------------------------------------------------
// Specify the configuration
#if defined(DEBUG) || defined(_DEBUG)
  #define NE_DEBUG
#endif

//----------------------------------------------------------------------
// Platform Headers
// Include platform specific header files.
#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
  #include "CoreFoundation/CoreFoundation.h"
  //#include <mach/mach_host.h>
#elif NE_PLATFORM == NE_PLATFORM_WIN32
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  #define NOMINMAX
  #include <windows.h>
#endif // NE_PLATFORM


//----------------------------------------------------------------------------
// Integer formats of fixed bit width
#ifndef _MSC_VER
  #include <inttypes.h>
  typedef uint64_t         uint64;
  typedef int64_t          int64;
  typedef uint32_t         uint32;
  typedef int32_t          int32;
  typedef uint16_t         uint16;
  typedef short            int16;
  typedef uint8_t          uint8;
  typedef char             int8;
#else
  typedef unsigned __int64 uint64;
  typedef signed __int64   int64;
  typedef unsigned int     uint32;
  typedef int              int32;
  typedef unsigned short   uint16;
  typedef short            int16;
  typedef unsigned char    uint8;
  typedef char             int8;
#endif


#endif // __NEBULAE_PLATFORM_H__