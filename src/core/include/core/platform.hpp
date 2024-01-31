/**
 * @brief Detect supported platforms using predefined macros.
 * XUZY_OS_WINDOWS  - Windows x64 (Desktop, MinGW, or Mobile)
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
/* Windows x64/x86 */
#  ifdef _WIN64
/* Windows x64  */
#    define XUZY_OS_WINDOWS
#  else
/* Windows x86 */
#    error "x86 Builds are not supported!"
#  endif
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
#  define XUZY_OS_CYGWIN
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define XUZY_OS_ANDROID
	#error "Android is not supported!"
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  ifndef XUZY_OS_LINUX
#    define XUZY_OS_LINUX
#  endif
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__) || \
    defined(__MACH__)
#  include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define XUZY_OS_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#  define XUZY_OS_MACOSX
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__FreeBSD__)
#  define XUZY_OS_FREEBSD
#elif defined(__NetBSD__)
#  define XUZY_OS_NETBSD
#elif defined(__OpenBSD__)
#  define XUZY_OS_OPENBSD
#elif defined(hpux) || defined(_hpux)
#  define XUZY_OS_HP
#else
/* Unknown compiler/platform */
#  error Platform not supported so far.
#endif // End of platform detection

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