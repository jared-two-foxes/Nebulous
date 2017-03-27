#ifndef __NEBULAE_COMMON_ASSERT_H__
#define __NEBULAE_COMMON_ASSERT_H__

//----- The breakpoint ----
#ifdef NE_DEBUG
  #if NE_PLATFORM == NE_PLATFORM_WIN32
    #define NE_BREAKPOINT __debugbreak()
  #elif NE_PLATFORM == NE_PLATFORM_ANDROID
    #include <signal.h>
    #define NE_BREAKPOINT raise(SIGINT)
  #elif NE_PLATFORM == NE_PLATFORM_APPLE_IOS
    #include <signal.h>
    #include <unistd.h>
    #define NE_BREAKPOINT raise(SIGINT)
  #elif NE_PLATFORM == NE_PLATFORM_APPLE
    #include <signal.h>
    #include <unistd.h>
    #define NE_BREAKPOINT raise(SIGINT)
  #endif
#else
  #define NE_BREAKPOINT
#endif

struct SourceInfo
{
  char* file;
  char* function;
  int   line;

  SourceInfo( char* file_, char* function_, int line_ ) 
  : file(file_), function(function_), line(line_) {}

};

#define NE_SOURCE_INFO SourceInfo( __FILE__, __FUNCTION__, __LINE__ )

class Assert
{
private:
  SourceInfo m_source;

  public:
    // logs the formatted message
    Assert(const SourceInfo& sourceInfo, const char* format, ...) 
      : m_source( sourceInfo )
    {
      va_list arglist;
      va_start(arglist, format);

      PrintLine( format, arglist );
    }

    Assert& Variable(const char* const name, bool var)        { PrintLine( "  o Variable %s = %s (bool)", name, var ? "true" : "false" ); return *this; } 
    Assert& Variable(const char* const name, char var)        { PrintLine( "  o Variable %s = %c (char)", name, var ); return *this; } 
    Assert& Variable(const char* const name, short var)       { PrintLine( "  o Variable %s = %hd (short)", name, var ); return *this; } 
    Assert& Variable(const char* const name, int var)         { PrintLine( "  o Variable %s = %d (int)", name, var ); return *this; } 
    Assert& Variable(const char* const name, long var)        { PrintLine( "  o Variable %s = %ld (long)", name, var ); return *this; } 
    Assert& Variable(const char* const name, float var)       { PrintLine( "  o Variable %s = %f (float)", name, var ); return *this; } 
    Assert& Variable(const char* const name, double var)      { PrintLine( "  o Variable %s = %f (double)", name, var ); return *this; } 
    Assert& Variable(const char* const name, char* var)       { PrintLine( "  o Variable %s = %s (string)", name, var ); return *this; } 
    Assert& Variable(const char* const name, std::string var) { PrintLine( "  o Variable %s = %s (string)", name, var.c_str() ); return *this; } 

    // generic
    template <typename T>
    Assert& Variable(const char* const name, const T& value)
    {
      printf( "  o Variable %s = %p (object)\n", name, &value );
      return *this;
    }

    void PrintLine( const char* format, ... ) 
    { 
      va_list arglist;
      va_start(arglist, format);

      PrintLine( format, arglist );
    }

    void PrintLine( const char* format, va_list arglist ) 
    { 
      printf( "%s(%d) [%s]: (ASSERT) ", m_source.file, m_source.line, m_source.function ); 
      vprintf( format, arglist );
      printf( "\n" );

      va_end( arglist );
    }

};

#ifdef NE_DEBUG
  #define NE_ASSERT_IMPL_VAR(variable)         .Variable(NE_STRINGIZE(variable), variable)
  #define NE_ASSERT_IMPL_VARS(...)             NE_EXPAND_ARGS(NE_ASSERT_IMPL_VAR, __VA_ARGS__), NE_BREAKPOINT)
  #define NE_ASSERT(condition, format, ...)    (condition) ? ((void)(0)) : (Assert(NE_SOURCE_INFO, "Assertion \"" #condition "\" failed. " format, __VA_ARGS__) NE_ASSERT_IMPL_VARS
#else
  #define NE_ASSERT_IMPL(...)                  
  #define NE_ASSERT(condition, format, ...)    NE_UNUSED(condition); NE_UNUSED(format); NE_UNUSED( NE_PASS_VA(__VA_ARGS__) ); NE_ASSERT_IMPL
#endif

#endif // __NEBULAE_COMMON_ASSERT_H__