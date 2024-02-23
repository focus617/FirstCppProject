#include "pch.h"

#include "classloader/shared_library_unix.hpp"

#include <dlfcn.h>

using namespace class_loader;

std::mutex SharedLibraryImpl::_mutex;

SharedLibraryImpl::SharedLibraryImpl() { _handle = 0; }

SharedLibraryImpl::~SharedLibraryImpl() {}

void SharedLibraryImpl::loadImpl(const std::string& path, int flags) {
  std::scoped_lock lock(_mutex);

  if (_handle) {
    std::string msg = "Library " + path + " already loaded.";
    LOG(ERROR) << "SharedLibrary: " << msg;
    throw xuzy::LibraryAlreadyLoadedException(msg);
  }

  int realFlags = RTLD_LAZY;
  if (flags & SHLIB_LOCAL_IMPL)
    realFlags |= RTLD_LOCAL;
  else
    realFlags |= RTLD_GLOBAL;

  if (!std::filesystem::exists(path)) {
    std::string msg = "Library " + path + " not exist.";
    LOG(ERROR) << "SharedLibrary: " << msg;
    throw xuzy::LibraryNotFoundException(msg);
  }

  _handle = dlopen(path.c_str(), realFlags);
  if (!_handle) {
    const char* err = dlerror();
    std::string msg = "Library " + path + " load error(" +
                      (err ? std::string(err) : "") + ").";
    LOG(ERROR) << "SharedLibrary: Library " << msg;
    throw xuzy::LibraryLoadException(msg);
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
#if defined(XUZY_OS_CYGWIN) || defined(XUZY_OS_WINDOWS)
  return "";
#else
  return "lib";
#endif
}

std::string SharedLibraryImpl::suffixImpl() {
#if defined(XUZY_OS_MACOSX)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.dylib";
#  else
  return ".dylib";
#  endif
#elif defined(XUZY_OS_HP)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.sl";
#  else
  return ".sl";
#  endif
#elif defined(XUZY_OS_CYGWIN) || defined(XUZY_OS_WINDOWS)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.dll";
#  else
  return ".dll";
#  endif
#elif defined(XUZY_OS_LINUX)
#  if defined(_DEBUG) && !defined(NO_SHARED_LIBRARY_DEBUG_SUFFIX)
  return "d.so";
#  else
  return ".so";
#  endif
#else
#  error Platform not supported so far.
#endif
}
