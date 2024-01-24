/**
 * @brief Detect supported platforms.
*/
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define XUZY_OS_WINDOWS
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
#  define XUZY_OS_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  ifndef XUZY_OS_LINUX
#    define XUZY_OS_LINUX
#  endif
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#  define XUZY_OS_MACOSX
#elif defined(__FreeBSD__)
#  define XUZY_OS_FREEBSD
#elif defined(__NetBSD__)
#  define XUZY_OS_NETBSD
#elif defined(__OpenBSD__)
#  define XUZY_OS_OPENBSD
#elif defined(__EMSCRIPTEN__)
#  define XUZY_OS_EMSCRIPTEN
#elif defined(hpux) || defined(_hpux)
#  define XUZY_OS_HP
#else
// Add other platforms.
#error Platform not supported so far. 
#endif


