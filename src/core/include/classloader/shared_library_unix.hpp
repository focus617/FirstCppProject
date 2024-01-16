#pragma once

#include <dlfcn.h>

#include <mutex>

#include "visibility_control.hpp"
#include "exception.hpp"

namespace class_loader {
///
/// \brief The SharedLibrary class dynamically loads shared libraries
/// at run-time.
///
class XUZY_API SharedLibraryImpl {
 protected:
  enum Flags { SHLIB_GLOBAL_IMPL = 1, SHLIB_LOCAL_IMPL = 2 };

  SharedLibraryImpl();
  ~SharedLibraryImpl();
  void loadImpl(const std::string& path, int flags);
  void unloadImpl();
  bool isLoadedImpl() const;

  const std::string& getPathImpl() const;

  bool hasSymbolImpl(const std::string& name);

  template <typename SymbolType>
  SymbolType* findSymbolImpl(const std::string& name);

  static std::string prefixImpl();
  static std::string suffixImpl();

  static bool setSearchPathImpl(const std::string& path);

 private:
  std::string _path;
  void* _handle;
  static std::mutex _mutex;
};

template <typename SymbolType>
SymbolType* SharedLibraryImpl::findSymbolImpl(const std::string& name) {
  std::scoped_lock lock(_mutex);

  SymbolType* symbol = nullptr;

  if (_handle) {
    symbol = reinterpret_cast<SymbolType*>(dlsym(_handle, name.c_str()));
    if (nullptr == symbol) {
      const char* errors = dlerror();
      throw xuzy::NotFoundException(errors ? std::string(errors) : name);
    }
  }
  return symbol;
}

}  // namespace class_loader