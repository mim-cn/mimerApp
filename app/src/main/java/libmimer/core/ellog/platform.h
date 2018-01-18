#ifndef PLATFORM_H
#define PLATFORM_H

// address the operating systems
#if defined(_WIN32) || defined(WIN32)
#  undef  WIN32
#  define WIN32   1
#  undef  _WIN32
#  define _WIN32  1
#endif

#if defined(_WIN64) || defined(WIN64)
#  undef  WIN64
#  define WIN64   1
#  undef  _WIN64
#  define _WIN64  1
#endif

#if defined(WIN64) || defined(WIN32) || defined(_Windows) || defined(__MINGW32__)
#  define OS_MSWIN
#elif defined (__linux) || defined (__linux__) || defined(_MSYS)
#  define OS_LINUX
#else
#error unsupported operating system
#endif

#ifndef UTIL_EXPORTS
#define UTIL_EXPORTS
#endif

/* Some compilers use a special export keyword */
#ifdef OS_MSWIN
#  ifdef UTIL_EXPORTS
#    define UTIL_API __declspec(dllexport)
#  else
#    define UTIL_API __declspec(dllimport)
#  endif
#else
#  if(defined(__GNUC__) && __GNUC__ >= 4)
#    define UTIL_API __attribute__ ((visibility("default")))
#  else
#    define UTIL_API
#  endif
#endif

/* By default C calling convention */
#ifndef UTIL_CALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define UTIL_CALL __cdecl
#else
#define UTIL_CALL
#endif
#endif /* UTIL_CALL */

#if defined(__MINGW32__) || defined(__CYGWIN32__)
    typedef char               int_8;
    typedef short              int_16;
    typedef long               int_32;
    typedef long long          int_64;
    typedef unsigned char      uint_8;
    typedef unsigned short     uint_16;
    typedef unsigned long      uint_32;
    typedef unsigned long long uint_64;
    typedef uint_8              u_char;
#elif defined(OS_MSWIN)
    typedef __int8  int_8;
    typedef __int16 int_16;
    typedef __int32 int_32;
    typedef __int64 int_64;
    typedef unsigned int       uint;
    typedef unsigned __int8    uint_8;
    typedef unsigned __int16   uint_16;
    typedef unsigned __int32   uint_32;
    typedef unsigned __int64   uint_64;
    typedef uint_8              u_char;
#elif defined(OS_LINUX)
    typedef char	             int_8;
    typedef short	             int_16;
    typedef int 	             int_32;
    typedef unsigned char      uint_8;
    typedef unsigned short     uint_16;
    typedef unsigned int       uint_32;
    typedef wchar_t	           WCHAR;
    #if defined(__x86_64)
        typedef long int_64;
        typedef unsigned long  uint_64;
    #else
        typedef long long int_64;
        typedef unsigned long long uint_64;
    #endif
#endif // OS_LINUX

#endif // PLATFORM_H
