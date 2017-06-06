#ifndef __NEBULAE_MACRO_H__
#define __NEBULAE_MACRO_H__

/// allows member functions to be made abstract. uses nonstandard C++ extensions provided by MSVC
#define NE_ABSTRACT    abstract
 
/// marks member functions as being an override of a base class virtual function. uses nonstandard C++ extensions provided by MSVC
#define NE_OVERRIDE    override
 
/// allows classes and member functions to be made sealed. uses nonstandard C++ extensions provided by MSVC
#define NE_FINAL       final

/// allows to emit pragmas from within macros
#define NE_PRAGMA(pragma)    __pragma(pragma)
 
/// support for C99 restrict keyword
#define NE_RESTRICT    __restrict
 
/// tells the compiler that the return value (RV) of a function is an object that will not be aliased with any other pointers
#define NE_RESTRICT_RV    __declspec(restrict)
 
/// tells the compiler that a function call does not modify or reference visible global state and only modifies the memory pointed to directly by pointer parameters
#define NE_NO_ALIAS    __declspec(noalias)
 
/// forces a function to be inlined
#define NE_INLINE    __forceinline
 
/// tells the compiler to never inline a particular function
#define NE_NO_INLINE    __declspec(noinline)
 
/// passes optimization hints to the compiler
#define NE_HINT(hint)    __assume(hint)
 
/// used in switch-statements whose default-case can never be reached, resulting in more optimal code
#define NE_NO_SWITCH_DEFAULT    NE_HINT(0)

// warnings suppression tools
#if NE_COMPILER == NE_COMPILER_MSVC
  #define NE_UNUSED(...)  __pragma(warning(suppress:4555))
#elif NE_COMPILER == NE_COMPILER_CLANG
  #define NE_UNUSED(...) {\
    _Pragma("clang diagnostic push");\
    _Pragma("clang diagnostic ignored \"-Wunused-value\"");\
    ((void)(__VA_ARGS__))\
    _Pragma("clang diagnostic pop");\
  }
#else
  #define NE_UNUSED(...) ((void)(__VA_ARGS__))
#endif

/// stringizes a string, even macros
#define NE_STRINGIZE_HELPER(token)    #token
#define NE_STRINGIZE(str)             NE_STRINGIZE_HELPER(str)

/// concatenates two strings, even when the strings are macros themselves
#define NE_JOIN(x, y)                    NE_JOIN_HELPER(x, y)
#define NE_JOIN_HELPER(x, y)             NE_JOIN_HELPER_HELPER(x, y)
#define NE_JOIN_HELPER_HELPER(x, y)      x##y

// VS 2010 still has this compiler bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
// NE_PASS_VA passes __VA_ARGS__ as multiple parameters to another macro, 
#if _MSC_VER >= 1400
#    define NE_LEFT_PARENTHESIS (
#    define NE_RIGHT_PARENTHESIS )
#    define NE_PASS_VA(...)                            NE_LEFT_PARENTHESIS __VA_ARGS__ NE_RIGHT_PARENTHESIS
#else
#    define NE_PASS_VA(...)                            (__VA_ARGS__)
#endif

// NE_VA_NUM_ARGS() is a very nifty macro to retrieve the number of arguments handed to a variable-argument macro, caps at 10 arguments.
// The very handy implementation which returns 0 when no arguments are found which is used below can be found at
// http://stackoverflow.com/questions/26682812/argument-counting-macro-with-zero-arguments-for-visualstudio-2010/26685339#26685339
#if _MSC_VER >= 1400
  #define NE_VA_NUM_ARGS_REVERSE_SEQUENCE            9, 8, 7, 6, 5, 4, 3, 2, 1, 0
  #define NE_VA_NUM_ARGS_ZERO_BASED(...)             NE_VA_NUM_ARGS_HELPER NE_LEFT_PARENTHESIS __VA_ARGS__, NE_VA_NUM_ARGS_REVERSE_SEQUENCE NE_RIGHT_PARENTHESIS
  #define NE_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N

  #define NE_AUGMENTER(...)                          unused, __VA_ARGS__
  #define NE_VA_NUM_ARGS(...)                        NE_VA_NUM_ARGS_ZERO_BASED( NE_AUGMENTER(__VA_ARGS__) )
#else
#    define NE_VA_NUM_ARGS(...)                      NE_VA_NUM_ARGS_HELPER(0, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#    define NE_VA_NUM_ARGS_HELPER(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#endif

// variadic macro "dispatching" the arguments to the correct macro.
// the number of arguments is found by using NE_NUM_ARGS(__VA_ARGS__)
#if _MSC_VER >= 1400
  #define NE_EXPAND_ARGS(op, ...)    NE_JOIN(NE_EXPAND_ARGS_, NE_VA_NUM_ARGS(__VA_ARGS__)) NE_LEFT_PARENTHESIS op, __VA_ARGS__ NE_RIGHT_PARENTHESIS
#else
  #define NE_EXPAND_ARGS(op, ...)    NE_JOIN(NE_EXPAND_ARGS_, NE_VA_NUM_ARGS(__VA_ARGS__)) ( op, __VA_ARGS__ )
#endif

/* This will call a macro on each argument passed in */
#define NE_EXPAND_ARGS_0( op )                                         
#define NE_EXPAND_ARGS_1( op, a1)                                      op(a1)
#define NE_EXPAND_ARGS_2( op, a1, a2)                                  op(a1) op(a2)
#define NE_EXPAND_ARGS_3( op, a1, a2, a3)                              op(a1) op(a2) op(a3)
#define NE_EXPAND_ARGS_4( op, a1, a2, a3, a4)                          op(a1) op(a2) op(a3) op(a4)
#define NE_EXPAND_ARGS_5( op, a1, a2, a3, a4, a5)                      op(a1) op(a2) op(a3) op(a4) op(a5)
#define NE_EXPAND_ARGS_6( op, a1, a2, a3, a4, a5, a6)                  op(a1) op(a2) op(a3) op(a4) op(a5) op(a6)
#define NE_EXPAND_ARGS_7( op, a1, a2, a3, a4, a5, a6, a7)              op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7)
#define NE_EXPAND_ARGS_8( op, a1, a2, a3, a4, a5, a6, a7, a8)          op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7) op(a8)
#define NE_EXPAND_ARGS_9( op, a1, a2, a3, a4, a5, a6, a7, a8, a9)      op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7) op(a8) op(a9)
#define NE_EXPAND_ARGS_10(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) op(a1) op(a2) op(a3) op(a4) op(a5) op(a6) op(a7) op(a8) op(a9) op(a10)


#ifdef WIN32
# ifndef NE_SHARED_LIBRARY // libsourcey.h
#   define NE_EXTERN __declspec(dllexport)
# else
#   define NE_EXTERN __declspec(dllimport)
# endif
#else
# define NE_EXTERN // nothing
#endif

#endif // NEBULAE_MACRO_H__