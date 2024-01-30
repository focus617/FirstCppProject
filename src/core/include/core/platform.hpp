/**
 * @brief Detect supported platforms.
 * XUZY_OS_WINDOWS  - Windows (Desktop, MinGW, or Mobile)
 * XUZY_OS_CYGWIN   - Cygwin
 * XUZY_OS_LINUX    - Linux
 * XUZY_OS_MACOSX   - Mac OS X
 * XUZY_OS_FREEBSD  - FreeBSD
 * XUZY_OS_NETBSD   - NetBSD
 * XUZY_OS_OPENBSD  - OpenBSD
 * XUZY_OS_HP       - HP-UX
 */
#pragma once
#include <stdlib.h>

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
#elif defined(hpux) || defined(_hpux)
#  define XUZY_OS_HP
#else
// Add other platforms.
#  error Platform not supported so far.
#endif

// Determines the version of gcc that is used to compile this.
#ifdef __GNUC__
// 40302 means version 4.3.2.
#  define GNU_GCC_VER_ \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif  // __GNUC__

namespace posix {

#ifdef XUZY_OS_WINDOWS
// Windows CE has no C library. This implementation provides a reasonable
// imitation of standard behaviour.
[[noreturn]] void Abort();
#else
[[noreturn]] inline void Abort() { abort(); }
#endif  // XUZY_OS_WINDOWS

}  // namespace posix