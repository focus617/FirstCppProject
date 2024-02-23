#pragma once

#include <mutex>

#include "visibility_control.hpp"

namespace class_loader {
///
/// \brief The SharedLibrary class dynamically loads shared libraries
/// at run-time.
///
class XUZY_API SharedLibraryImpl {
 protected:
  SharedLibraryImpl();
  ~SharedLibraryImpl();
  void loadImpl(const std::string& path, int flags);
  void unloadImpl();
  bool isLoadedImpl() const;
  void* findSymbolImpl(const std::string& name);
  const std::string& getPathImpl() const;
  static std::string suffixImpl();
  static bool setSearchPathImpl(const std::string& path);

 private:
  std::string _path;
  void* _handle;
  static std::mutex _mutex;
};

}  // namespace class_loader