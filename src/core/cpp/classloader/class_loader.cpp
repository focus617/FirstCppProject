#include "classloader/class_loader.hpp"

#include <glog/logging.h>

#include "classloader/shared_library.hpp"
#include "exception.hpp"

namespace class_loader {

bool ClassLoader::has_unmananged_instance_been_created_ = false;

bool ClassLoader::hasUnmanagedInstanceBeenCreated() {
  return ClassLoader::has_unmananged_instance_been_created_;
}

ClassLoader::ClassLoader(const std::string& library_path,
                         bool ondemand_load_unload)
    : ondemand_load_unload_(ondemand_load_unload),
      library_path_(library_path),
      load_ref_count_(0),
      plugin_ref_count_(0) {
  LOG(INFO) << "class_loader::ClassLoader: Constructing new ClassLoader("
            << this << ") bound to library " << library_path << ".";

  if (!isOnDemandLoadUnloadEnabled()) {
    loadLibrary();
  }
}

ClassLoader::~ClassLoader() {
  LOG(INFO) << "class_loader::ClassLoader: Destroying class loader, unloading "
               "associated library...";
  if (isLibraryLoaded()) {
    unloadLibrary();
  }
}

bool ClassLoader::isLibraryLoaded() {
  return class_loader::impl::isLibraryLoaded(getLibraryPath(), this);
}

bool ClassLoader::isLibraryLoadedByAnyClassloader() {
  return class_loader::impl::isLibraryLoadedByAnybody(getLibraryPath());
}

void ClassLoader::loadLibrary() {
  std::scoped_lock lock(load_ref_count_mutex_);

  load_ref_count_ = load_ref_count_ + 1;

  class_loader::impl::loadLibrary(getLibraryPath(), this);
}

int ClassLoader::unloadLibrary() { return unloadLibraryInternal(true); }

int ClassLoader::unloadLibraryInternal(bool lock_plugin_ref_count) {
  std::scoped_lock load_ref_lock(load_ref_count_mutex_);
  std::scoped_lock plugin_ref_lock(plugin_ref_count_mutex_);

  // if (lock_plugin_ref_count)
  // {
  //     plugin_ref_lock =
  //     std::scoped_lock<std::recursive_mutex>(plugin_ref_count_mutex_);
  // }

  if (plugin_ref_count_ > 0) {
    LOG(WARNING)
        << "class_loader.ClassLoader: SEVERE WARNING!!!\n"
        << "Attempting to unload " << library_path_ << "\n"
        << "while objects created by this library still exist in the heap!\n"
        << "You should delete your objects before destroying the ClassLoader. "
        << "The library will NOT be unloaded.";
  } else {
    load_ref_count_ = load_ref_count_ - 1;
    if (0 == load_ref_count_) {
      class_loader::impl::unloadLibrary(getLibraryPath(), this);
    } else if (load_ref_count_ < 0) {
      load_ref_count_ = 0;
    }
  }
  return load_ref_count_;
}

}  // namespace class_loader