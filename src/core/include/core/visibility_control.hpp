#pragma once

#if defined(__GNUC__)
#  define XUZY_DEPRECATED __attribute__((deprecated))
#  define XUZY_FORCE_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#  define XUZY_DEPRECATED
#  define XUZY_FORCE_INLINE __forceinline
#else
#  define XUZY_DEPRECATED
#  define XUZY_FORCE_INLINE inline
#endif

// Ignore warnings about import/exports when deriving from std classes.
#if defined(_MSC_VER)
#  pragma warning(disable : 4251)
#  pragma warning(disable : 4275)
#endif

// This logic was borrowed (then namespaced) from the examples on the
// gcc wiki:  https://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined(_WIN32) || defined(__CYGWIN__)
#  define XUZY_HELPER_DLL_IMPORT __declspec(dllimport)
#  define XUZY_HELPER_DLL_EXPORT __declspec(dllexport)
#  define XUZY_HELPER_DLL_LOCAL
#else
#  if __GNUC__ >= 4
#    define XUZY_HELPER_DLL_IMPORT __attribute__((visibility("default")))
#    define XUZY_HELPER_DLL_EXPORT __attribute__((visibility("default")))
#    define XUZY_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
#  else
#    define XUZY_HELPER_DLL_IMPORT
#    define XUZY_HELPER_DLL_EXPORT
#    define XUZY_HELPER_DLL_LOCAL
#  endif
#endif

// Now we use the generic helper definitions above to define
// XUZY_API and XUZY_LOCAL.
// -  XUZY_API is used for the public API symbols. It either DLL imports
//    or DLL exports (or does nothing for static build)
// -  XUZY_LOCAL is used for non-api symbols.

#ifdef XUZY_DLL            // defined if module is compiled as a DLL
#  ifdef XUZY_DLL_EXPORTS  // defined if we are building the DLL
#    define XUZY_API XUZY_HELPER_DLL_EXPORT
#  else  // if we are using it
#    define XUZY_API XUZY_HELPER_DLL_IMPORT
#  endif  // XUZY_DLL_EXPORTS
#  define XUZY_LOCAL XUZY_HELPER_DLL_LOCAL
#else  // XUZY_DLL is not defined: this means FOX is a static lib.
#  define XUZY_API
#  define XUZY_LOCAL
#endif  // XUZY_DLL