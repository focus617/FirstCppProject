#include "pch.h"

#include "classloader/multi_library_class_loader.hpp"

#include <cstddef>

namespace xuzy {
XUZY_IMPLEMENT_EXCEPTION(NoClassLoaderExistsException, RuntimeException,
                         "Cannot create class")
}

namespace class_loader {

MultiLibraryClassLoader::MultiLibraryClassLoader(
    bool enable_ondemand_loadunload)
    : enable_ondemand_loadunload_(enable_ondemand_loadunload) {}

MultiLibraryClassLoader::~MultiLibraryClassLoader() {
  shutdownAllClassLoaders();
}

std::vector<std::string> MultiLibraryClassLoader::getRegisteredLibraries() {
  std::vector<std::string> libraries;
  for (auto& it : active_class_loaders_) {
    if (it.second != nullptr) {
      libraries.push_back(it.first);
    }
  }
  return libraries;
}

ClassLoader* MultiLibraryClassLoader::getClassLoaderForLibrary(
    const std::string& library_path) {
  LibraryToClassLoaderMap::iterator itr =
      active_class_loaders_.find(library_path);
  if (itr != active_class_loaders_.end()) {
    return itr->second;
  } else {
    return nullptr;
  }
}

ClassLoaderVector MultiLibraryClassLoader::getAllAvailableClassLoaders() {
  ClassLoaderVector loaders;
  for (auto& it : active_class_loaders_) {
    loaders.push_back(it.second);
  }
  return loaders;
}

bool MultiLibraryClassLoader::isLibraryAvailable(
    const std::string& library_name) {
  return getClassLoaderForLibrary(library_name) != nullptr;
}

void MultiLibraryClassLoader::loadLibrary(const std::string& library_path) {
  if (!isLibraryAvailable(library_path)) {
    active_class_loaders_[library_path] = new class_loader::ClassLoader(
        library_path, isOnDemandLoadUnloadEnabled());
  }
}

void MultiLibraryClassLoader::shutdownAllClassLoaders() {
  std::vector<std::string> available_libraries = getRegisteredLibraries();

  for (auto& library_path : getRegisteredLibraries()) {
    unloadLibrary(library_path);
  }
}

int MultiLibraryClassLoader::unloadLibrary(const std::string& library_path) {
  int remaining_unloads = 0;
  LibraryToClassLoaderMap::iterator itr =
      active_class_loaders_.find(library_path);
  if (itr != active_class_loaders_.end()) {
    ClassLoader* loader = itr->second;
    if (0 == (remaining_unloads = loader->unloadLibrary())) {
      delete (loader);
      active_class_loaders_.erase(itr);
    }
  }
  return remaining_unloads;
}

}  // namespace class_loader
