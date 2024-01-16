#include "classloader/shared_library.hpp"

#include <glog/logging.h>

using namespace class_loader;

SharedLibrary::SharedLibrary() {}

SharedLibrary::SharedLibrary(const std::string& path) {
  LOG(INFO) << "class_loader::SharedLibrary: Constructing new SharedLibrary("
            << this << ") bound to library " << path << ".";

  loadImpl(path, 0);
}

SharedLibrary::SharedLibrary(const std::string& path, int flags) {
  loadImpl(path, flags);
}

SharedLibrary::~SharedLibrary() {
  LOG(INFO) << "class_loader::SharedLibrary: Destroying SharedLibrary, the "
               "actual library remains loaded....";
}

void SharedLibrary::load(const std::string& path) { loadImpl(path, 0); }

void SharedLibrary::load(const std::string& path, int flags) {
  loadImpl(path, flags);
}

void SharedLibrary::unload() { unloadImpl(); }

bool SharedLibrary::isLoaded() const { return isLoadedImpl(); }

const std::string& SharedLibrary::getPath() const { return getPathImpl(); }

bool SharedLibrary::setSearchPath(const std::string& path) {
  return setSearchPathImpl(path);
}

bool SharedLibrary::hasSymbol(const std::string& name) {
  return hasSymbolImpl(name);
}

std::string SharedLibrary::systemLibraryPrefix() { return prefixImpl(); }

std::string SharedLibrary::systemLibrarySuffix() { return suffixImpl(); }

std::string SharedLibrary::systemLibraryFormat(
    const std::string& library_name) {
  return systemLibraryPrefix() + library_name + systemLibrarySuffix();
}