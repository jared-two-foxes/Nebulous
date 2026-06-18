#ifndef __NEBULAE_COMMON_ASSERT_H__
#define __NEBULAE_COMMON_ASSERT_H__

#include <Nebulae/Common/Base/Log/Log.h>

//----- The breakpoint ----
#ifdef NE_DEBUG
#if NE_PLATFORM == NE_PLATFORM_WIN32
#define NE_BREAKPOINT __debugbreak()
#elif NE_PLATFORM == NE_PLATFORM_ANDROID
#include <signal.h>
#define NE_BREAKPOINT raise( SIGINT )
#elif NE_PLATFORM == NE_PLATFORM_APPLE_IOS
#include <signal.h>
#include <unistd.h>
#define NE_BREAKPOINT raise( SIGINT )
#elif NE_PLATFORM == NE_PLATFORM_APPLE
#include <signal.h>
#include <unistd.h>
#define NE_BREAKPOINT raise( SIGINT )
#endif
#else
#define NE_BREAKPOINT
#endif

// Keep Assert class for backward compatibility with template specializations
// (e.g., Vector2.h, Vector4.h have template specializations of Assert::Variable)
class Assert
{
public:
  // Dummy constructor for compatibility
  Assert() {}

  // Dummy Variable methods for template specializations
  template <typename T> Assert& Variable( const char* const name, const T& value ) { return *this; }
};

// Keep NE_SOURCE_INFO for backward compatibility
#define NE_SOURCE_INFO ""

#ifdef NE_DEBUG
#define NE_ASSERT( condition, ... )                                 \
  do                                                                \
  {                                                                 \
    if ( !( condition ) )                                           \
    {                                                               \
      NE_LOG_ERROR( "Assert", "Assertion failed: {}", #condition ); \
      __VA_OPT__( NE_LOG_ERROR( "Assert", __VA_ARGS__ ); )          \
      NE_BREAKPOINT;                                                \
    }                                                               \
  } while ( 0 )
#else
#define NE_ASSERT( condition, ... ) ( (void)0 )
#endif

#endif // __NEBULAE_COMMON_ASSERT_H__
