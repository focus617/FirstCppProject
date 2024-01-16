#include "classloader/shared_library_unix.hpp"

#include <dlfcn.h>
#include <glog/logging.h>

#include "exception.hpp"

using namespace class_loader;

std::mutex SharedLibraryImpl::_mutex;

SharedLibraryImpl::SharedLibraryImpl() { _handle = 0; }

SharedLibraryImpl::~SharedLibraryImpl() {}

void SharedLibraryImpl::loadImpl(const std::string& path, int flags) {
  std::scoped_lock lock(_mutex);

  if (_handle) throw xuzy::LibraryAlreadyLoadedException(path);

  int realFlags = RTLD_LAZY;
  if (flags & SHLIB_LOCAL_IMPL)
    realFlags |= RTLD_LOCAL;
  else
    realFlags |= RTLD_GLOBAL;

  _handle = dlopen(path.c_str(), realFlags);
  if (!_handle) {
    const char* err = dlerror();
    throw xuzy::LibraryLoadException(err ? std::string(err) : path);
  }
  _path = path;
}

void SharedLibraryImpl::unloadImpl() {
  std::scoped_lock lock(_mutex);

  if (_handle) {
    dlclose(_handle);
    _handle = 0;
  }
}

bool SharedLibraryImpl::isLoadedImpl() const {
  std::scoped_lock lock(_mutex);
  return _handle != 0;
}

const std::string& SharedLibraryImpl::getPathImpl() const { return _path; }

bool SharedLibraryImpl::setSearchPathImpl(const std::string&) { return false; }

bool SharedLibraryImpl::hasSymbolImpl(const std::string& name) {
  std::scoped_lock lock(_mutex);

  bool result = false;
  if (_handle) {
    result = dlsym(_handle, name.c_str()) ? true : false;
  }
  return result;
}

std::string SharedLibraryImpl::prefixImpl() {
#if defined(__CYGWIN__) || defined(_WIN32)
  return "";
#else
  return "lib";
#endif
}

std::string SharedLibraryImpl::suffixImpl() {
#if defined(__APPLE__)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.dylib";
#  else
  return ".dylib";
#  endif
#elif defined(hpux) || defined(_hpux)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.sl";
#  else
  return ".sl";
#  endif
#elif defined(__CYGWIN__) || defined(_WIN32)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.dll";
#  else
  return ".dll";
#  endif
#else
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.so";
#  else
  return ".so";
#  endif
#endif
}
