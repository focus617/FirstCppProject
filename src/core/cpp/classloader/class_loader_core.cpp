#include "classloader/class_loader_core.hpp"

#include <glog/logging.h>

#include <cassert>
#include <vector>

#include "classloader/class_loader.hpp"
#include "classloader/shared_library.hpp"
#include "exception.hpp"

namespace xuzy {
XUZY_IMPLEMENT_EXCEPTION(CreateClassException, RuntimeException,
                         "Cannot create class")
}

namespace class_loader {
namespace impl {

// Global data

BaseToFactoryMapMap& getGlobalPluginBaseToFactoryMapMap() {
  static BaseToFactoryMapMap instance;
  return instance;
}

LibraryVector& getLoadedLibraryVector() {
  static LibraryVector instance;
  return instance;
}

std::string& getCurrentlyLoadingLibraryNameReference() {
  static std::string library_name;
  return library_name;
}

std::string getCurrentlyLoadingLibraryName() {
  return getCurrentlyLoadingLibraryNameReference();
}

void setCurrentlyLoadingLibraryName(const std::string& library_name) {
  std::string& library_name_ref = getCurrentlyLoadingLibraryNameReference();

  library_name_ref = library_name;
}

ClassLoader*& getCurrentlyActiveClassLoaderReference() {
  static ClassLoader* loader = nullptr;
  return loader;
}

ClassLoader* getCurrentlyActiveClassLoader() {
  return getCurrentlyActiveClassLoaderReference();
}

void setCurrentlyActiveClassLoader(ClassLoader* loader) {
  ClassLoader*& loader_ref = getCurrentlyActiveClassLoaderReference();
  loader_ref = loader;
}

FactoryMap& getFactoryMapForBaseClass(
    const std::string& typeid_base_class_name) {
  BaseToFactoryMapMap& factoryMapMap = getGlobalPluginBaseToFactoryMapMap();

  std::string base_class_name = typeid_base_class_name;
  if (factoryMapMap.find(base_class_name) == factoryMapMap.end()) {
    factoryMapMap[base_class_name] = FactoryMap();
  }

  return factoryMapMap[base_class_name];
}

MetaObjectVector& getMetaObjectGraveyard() {
  static MetaObjectVector instance;
  return instance;
}

std::recursive_mutex& getLoadedLibraryVectorMutex() {
  static std::recursive_mutex m;
  return m;
}

std::recursive_mutex& getPluginBaseToFactoryMapMapMutex() {
  static std::recursive_mutex m;
  return m;
}

bool& hasANonPurePluginLibraryBeenOpenedReference() {
  static bool hasANonPurePluginLibraryBeenOpenedReference = false;
  return hasANonPurePluginLibraryBeenOpenedReference;
}

bool hasANonPurePluginLibraryBeenOpened() {
  return hasANonPurePluginLibraryBeenOpenedReference();
}

void setANonPurePluginLibraryBeenOpened(bool hasIt) {
  hasANonPurePluginLibraryBeenOpenedReference() = hasIt;
}

// MetaObject search/insert/removal/query

MetaObjectVector allMetaObjects(const FactoryMap& factories) {
  MetaObjectVector all_meta_objs;
  for (auto& it : factories) {
    all_meta_objs.push_back(it.second);
  }
  return all_meta_objs;
}

MetaObjectVector allMetaObjects() {
  std::scoped_lock lock(getPluginBaseToFactoryMapMapMutex());

  MetaObjectVector all_meta_objs;
  BaseToFactoryMapMap& factory_map_map = getGlobalPluginBaseToFactoryMapMap();

  BaseToFactoryMapMap::iterator itr;
  for (auto& it : factory_map_map) {
    MetaObjectVector objs = allMetaObjects(it.second);
    all_meta_objs.insert(all_meta_objs.end(), objs.begin(), objs.end());
  }
  return all_meta_objs;
}

MetaObjectVector filterAllMetaObjectsOwnedBy(const MetaObjectVector& to_filter,
                                             const ClassLoader* owner) {
  MetaObjectVector filtered_objs;
  for (auto& f : to_filter) {
    if (f->isOwnedBy(owner)) {
      filtered_objs.push_back(f);
    }
  }
  return filtered_objs;
}

MetaObjectVector filterAllMetaObjectsAssociatedWithLibrary(
    const MetaObjectVector& to_filter, const std::string& library_path) {
  MetaObjectVector filtered_objs;
  for (auto& f : to_filter) {
    if (f->getAssociatedLibraryPath() == library_path) {
      filtered_objs.push_back(f);
    }
  }
  return filtered_objs;
}

MetaObjectVector allMetaObjectsForClassLoader(const ClassLoader* owner) {
  return filterAllMetaObjectsOwnedBy(allMetaObjects(), owner);
}

MetaObjectVector allMetaObjectsForLibrary(const std::string& library_path) {
  return filterAllMetaObjectsAssociatedWithLibrary(allMetaObjects(),
                                                   library_path);
}

MetaObjectVector allMetaObjectsForLibraryOwnedBy(
    const std::string& library_path, const ClassLoader* owner) {
  return filterAllMetaObjectsOwnedBy(allMetaObjectsForLibrary(library_path),
                                     owner);
}

void insertMetaObjectIntoGraveyard(AbstractMetaObjectBase* meta_obj) {
  LOG(INFO) << "class_loader.impl: Inserting MetaObject (class = "
            << meta_obj->className()
            << ", base_class = " << meta_obj->baseClassName()
            << ", ptr = " << reinterpret_cast<void*>(meta_obj)
            << ") into graveyard";

  getMetaObjectGraveyard().push_back(meta_obj);
}

void destroyMetaObjectsForLibrary(const std::string& library_path,
                                  FactoryMap& factories,
                                  const ClassLoader* loader) {
  FactoryMap::iterator factory_itr = factories.begin();
  while (factory_itr != factories.end()) {
    AbstractMetaObjectBase* meta_obj = factory_itr->second;
    if (meta_obj->getAssociatedLibraryPath() == library_path &&
        meta_obj->isOwnedBy(loader)) {
      meta_obj->removeOwningClassLoader(loader);
      if (!meta_obj->isOwnedByAnybody()) {
        FactoryMap::iterator factory_itr_copy = factory_itr;
        ++factory_itr;
        // TODO(mikaelarguedas) fix this when branching out for
        // melodic
        // Note: map::erase does not return iterator like
        // vector::erase does.
        // Hence the ugliness of this code and a need for copy.
        // Should be fixed in next C++ revision
        factories.erase(factory_itr_copy);

        // Insert into graveyard
        // We remove the metaobject from its factory map,
        // but we don't destroy it...instead itsaved to a
        // "graveyard" to the side.
        // This is due to our static global variable
        // initialization problem that causes factories
        // to not be registered when a library is closed and
        // then reopened.
        // This is because it's truly not closed due to the use
        // of global symbol binding i.e. calling dlopen with
        // RTLD_GLOBAL instead of RTLD_LOCAL.
        // We require using the former as the which is required
        // to support RTTI
        insertMetaObjectIntoGraveyard(meta_obj);
      } else {
        ++factory_itr;
      }
    } else {
      ++factory_itr;
    }
  }
}

void destroyMetaObjectsForLibrary(const std::string& library_path,
                                  const ClassLoader* loader) {
  std::scoped_lock lock(getPluginBaseToFactoryMapMapMutex());

  LOG(INFO) << "class_loader.impl: "
            << "Removing MetaObjects associated with library " << library_path
            << " and class loader " << reinterpret_cast<const void*>(loader)
            << " from global plugin-to-factorymap map.\n";

  // We have to walk through all FactoryMaps to be sure
  BaseToFactoryMapMap& factory_map_map = getGlobalPluginBaseToFactoryMapMap();
  for (auto& it : factory_map_map) {
    destroyMetaObjectsForLibrary(library_path, it.second, loader);
  }

  LOG(INFO) << "class_loader.impl: Metaobjects removed.";
}

bool areThereAnyExistingMetaObjectsForLibrary(const std::string& library_path) {
  return allMetaObjectsForLibrary(library_path).size() > 0;
}

std::vector<std::string> getAllLibrariesUsedByClassLoader(
    const ClassLoader* loader) {
  MetaObjectVector all_loader_meta_objs = allMetaObjectsForClassLoader(loader);

  std::vector<std::string> all_libs;
  for (auto& meta_obj : all_loader_meta_objs) {
    std::string lib_path = meta_obj->getAssociatedLibraryPath();
    if (std::find(all_libs.begin(), all_libs.end(), lib_path) ==
        all_libs.end()) {
      all_libs.push_back(lib_path);
    }
  }
  return all_libs;
}

// Implementation of Remaining Core plugin impl Functions

void addClassLoaderOwnerForAllExistingMetaObjectsForLibrary(
    const std::string& library_path, ClassLoader* loader) {
  MetaObjectVector all_meta_objs = allMetaObjectsForLibrary(library_path);
  for (auto& meta_obj : all_meta_objs) {
    LOG(INFO) << "class_loader.impl: Tagging existing MetaObject "
              << reinterpret_cast<void*>(meta_obj)
              << " (base = " << meta_obj->baseClassName()
              << ", derived = " << meta_obj->className()
              << ") with class loader " << reinterpret_cast<void*>(loader)
              << " (library path = "
              << (nullptr == loader ? loader->getLibraryPath() : "NULL")
              << ").";

    meta_obj->addOwningClassLoader(loader);
  }
}

void revivePreviouslyCreateMetaobjectsFromGraveyard(
    const std::string& library_path, ClassLoader* loader) {
  std::scoped_lock b2fmm_lock(getPluginBaseToFactoryMapMapMutex());
  MetaObjectVector& graveyard = getMetaObjectGraveyard();

  for (auto& obj : graveyard) {
    if (obj->getAssociatedLibraryPath() == library_path) {
      LOG(INFO) << "class_loader.impl: "
                << "Resurrected factory metaobject from graveyard, class = "
                << obj->className()
                << " , base_class = " << obj->baseClassName()
                << " ptr = " << reinterpret_cast<void*>(obj) << "..."
                << "bound to ClassLoader " << reinterpret_cast<void*>(loader)
                << " (library path = "
                << (nullptr == loader ? loader->getLibraryPath() : "NULL")
                << ")";

      obj->addOwningClassLoader(loader);
      assert(obj->typeidBaseClassName() != "UNSET");

      FactoryMap& factory =
          getFactoryMapForBaseClass(obj->typeidBaseClassName());

      factory[obj->className()] = obj;
    }
  }
}

void purgeGraveyardOfMetaobjects(const std::string& library_path,
                                 ClassLoader* loader, bool delete_objs) {
  MetaObjectVector all_meta_objs = allMetaObjects();
  // Note: Lock must happen after call to allMetaObjects as that will lock
  std::scoped_lock b2fmm_lock(getPluginBaseToFactoryMapMapMutex());

  MetaObjectVector& graveyard = getMetaObjectGraveyard();
  MetaObjectVector::iterator itr = graveyard.begin();

  while (itr != graveyard.end()) {
    AbstractMetaObjectBase* obj = *itr;
    if (obj->getAssociatedLibraryPath() == library_path) {
      LOG(INFO) << "class_loader.impl: "
                << "Purging factory metaobject from graveyard, class ="
                << obj->className() << ", base_class = " << obj->baseClassName()
                << " ptr = " << reinterpret_cast<void*>(obj)
                << "...bound to ClassLoader " << reinterpret_cast<void*>(loader)
                << " (library path = "
                << (nullptr == loader ? loader->getLibraryPath() : "NULL")
                << ")";

      bool is_address_in_graveyard_same_as_global_factory_map =
          std::find(all_meta_objs.begin(), all_meta_objs.end(), *itr) !=
          all_meta_objs.end();
      itr = graveyard.erase(itr);
      if (delete_objs) {
        if (is_address_in_graveyard_same_as_global_factory_map) {
          LOG(INFO) << "class_loader.impl: "
                    << "Newly created metaobject factory in global factory map "
                       "map has same address as "
                    << "one in graveyard -- metaobject has been purged from "
                       "graveyard but not deleted.";
        } else {
          assert(hasANonPurePluginLibraryBeenOpened() == false);

          LOG(INFO) << "class_loader.impl: "
                    << "Also destroying metaobject "
                    << reinterpret_cast<void*>(obj)
                    << " (class = " << obj->className()
                    << ", base_class = " << obj->baseClassName()
                    << ", library_path = " << obj->getAssociatedLibraryPath()
                    << ") in addition to purging it from graveyard.";

#ifndef _WIN32
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
          delete (obj);  // Note: This is the only place where metaobjects can
                         // be destroyed
#ifndef _WIN32
#  pragma GCC diagnostic pop
#endif
        }
      }
    } else {
      ++itr;
    }
  }
}

// Loaded Library Vector manipulation

LibraryVector::iterator findLoadedLibrary(const std::string& library_path) {
  LibraryVector& open_libraries = getLoadedLibraryVector();
  for (auto it = open_libraries.begin(); it != open_libraries.end(); ++it) {
    if (it->first == library_path) {
      return it;
    }
  }
  return open_libraries.end();
}

bool isLibraryLoaded(const std::string& library_path, ClassLoader* loader) {
  bool is_lib_loaded_by_anyone = isLibraryLoadedByAnybody(library_path);
  size_t num_meta_objs_for_lib = allMetaObjectsForLibrary(library_path).size();
  size_t num_meta_objs_for_lib_bound_to_loader =
      allMetaObjectsForLibraryOwnedBy(library_path, loader).size();
  bool are_meta_objs_bound_to_loader =
      (0 == num_meta_objs_for_lib)
          ? true
          : (num_meta_objs_for_lib_bound_to_loader <= num_meta_objs_for_lib);

  return is_lib_loaded_by_anyone && are_meta_objs_bound_to_loader;
}

bool isLibraryLoadedByAnybody(const std::string& library_path) {
  std::scoped_lock lock(getLoadedLibraryVectorMutex());

  LibraryVector& open_libraries = getLoadedLibraryVector();
  LibraryVector::iterator itr = findLoadedLibrary(library_path);

  if (itr != open_libraries.end()) {
    // Ensure Shared_Library actually thinks the library is loaded
    assert(itr->second->isLoaded() == true);
    return true;
  } else {
    return false;
  }
}

void loadLibrary(const std::string& library_path, ClassLoader* loader) {
  static std::recursive_mutex loader_mutex;
  LOG(INFO) << "class_loader.impl: Attempting to load library " << library_path
            << " on behalf of ClassLoader handle "
            << reinterpret_cast<void*>(loader) << "...\n";

  std::scoped_lock loader_lock(loader_mutex);

  // If it's already open, just update existing metaobjects to
  // have an additional owner.
  if (isLibraryLoadedByAnybody(library_path)) {
    std::scoped_lock lock(getPluginBaseToFactoryMapMapMutex());

    LOG(INFO) << "class_loader.impl: Library already in memory, but binding "
                 "existing MetaObjects to loader if necesesary.";

    addClassLoaderOwnerForAllExistingMetaObjectsForLibrary(library_path,
                                                           loader);
    return;
  }

  SharedLibrary* library_handle = nullptr;

  {
    try {
      setCurrentlyActiveClassLoader(loader);
      setCurrentlyLoadingLibraryName(library_path);
      library_handle = new SharedLibrary(library_path);
    } catch (const xuzy::LibraryAlreadyLoadedException& e) {
      setCurrentlyLoadingLibraryName("");
      setCurrentlyActiveClassLoader(nullptr);

      std::string msg = "Library " + library_path + " already loaded (" +
                        std::string(e.message()) + ")";
      LOG(ERROR) << "class_loader.impl: " << msg;
      throw xuzy::LibraryAlreadyLoadedException(msg);

    } catch (const xuzy::LibraryNotFoundException& e) {
      setCurrentlyLoadingLibraryName("");
      setCurrentlyActiveClassLoader(nullptr);

      std::string msg = "Library " + library_path + " not found (" +
                        std::string(e.message()) + ")";
      LOG(ERROR) << "class_loader.impl: " << msg;
      throw xuzy::LibraryNotFoundException(msg);

    } catch (const xuzy::LibraryLoadException& e) {
      setCurrentlyLoadingLibraryName("");
      setCurrentlyActiveClassLoader(nullptr);

      std::string msg = "Could not load library " + library_path + "(" +
                        std::string(e.message()) + ")";
      LOG(ERROR) << "class_loader.impl: " << msg;
      throw xuzy::LibraryLoadException(msg);

    } catch (xuzy::Exception& e) {
      LOG(ERROR) << "class_loader.impl: Exception " << e.what()
                 << "\nMessage: " << e.message() << std::endl;
    }

    setCurrentlyLoadingLibraryName("");
    setCurrentlyActiveClassLoader(nullptr);
  }

  assert(library_handle != nullptr);
  LOG(INFO) << "class_loader.impl: Successfully loaded library " << library_path
            << " into memory (SharedLibrary handle = "
            << reinterpret_cast<void*>(library_handle) << ").";

  // Graveyard scenario
  size_t num_lib_objs = allMetaObjectsForLibrary(library_path).size();
  if (0 == num_lib_objs) {
    LOG(INFO)
        << "class_loader.impl: Though the library " << library_path
        << " was just loaded, it seems no factory metaobjects were registered. "
           "\n"
        << "Checking factory graveyard for previously loaded metaobjects...";
    revivePreviouslyCreateMetaobjectsFromGraveyard(library_path, loader);
    // Note: The 'false' indicates we don't want to invoke delete on the
    // metaobject
    purgeGraveyardOfMetaobjects(library_path, loader, false);
  } else {
    LOG(INFO) << "class_loader.impl: Library " << library_path
              << " generated new factory metaobjects on load.\n"
              << "Destroying graveyarded objects from previous loads...";
    purgeGraveyardOfMetaobjects(library_path, loader, true);
  }

  // Insert library into global loaded library vector
  std::scoped_lock llv_lock(getLoadedLibraryVectorMutex());
  LibraryVector& open_libraries = getLoadedLibraryVector();
  // Note: SharedLibrary automatically calls load() when library passed to
  // constructor
  open_libraries.push_back(LibraryPair(library_path, library_handle));
}

void unloadLibrary(const std::string& library_path, ClassLoader* loader) {
  if (hasANonPurePluginLibraryBeenOpened()) {
    LOG(INFO)
        << "class_loader.impl: "
        << "Cannot unload " << library_path
        << " or ANY other library as a non-pure plugin library was opened. "
        << "As class_loader has no idea which libraries class factories were "
           "exported from, it can safely close any library without potentially "
           "unlinking symbols that are still actively being used. "
        << "You must refactor your plugin libraries to be made exclusively of "
           "plugins in order for this error to stop happening.";
  } else {
    LOG(INFO) << "class_loader.impl: "
              << "Unloading library " << library_path
              << " on behalf of ClassLoader " << reinterpret_cast<void*>(loader)
              << "...";

    std::scoped_lock lock(getLoadedLibraryVectorMutex());
    LibraryVector& open_libraries = getLoadedLibraryVector();
    LibraryVector::iterator itr = findLoadedLibrary(library_path);

    if (itr != open_libraries.end()) {
      SharedLibrary* library = itr->second;
      std::string lib_path = itr->first;

      try {
        destroyMetaObjectsForLibrary(lib_path, loader);

        // Remove from loaded library list as well if no more factories
        // associated with said library
        if (!areThereAnyExistingMetaObjectsForLibrary(lib_path)) {
          LOG(INFO) << "class_loader.impl: "
                    << "There are no more MetaObjects left for " << lib_path
                    << " so unloading library and removing from loaded library "
                       "vector.\n";

          library->unload();
          assert(library->isLoaded() == false);
          delete (library);
          itr = open_libraries.erase(itr);
        } else {
          LOG(INFO) << "class_loader.impl: "
                    << "MetaObjects still remain in memory meaning other "
                       "ClassLoaders are still using library, keeping library "
                    << lib_path << " open.";
        }
        return;
      } catch (const xuzy::RuntimeException& e) {
        delete (library);
        throw xuzy::LibraryUnLoadException("Could not unload library (" +
                                           std::string(e.message()) + ")");
      }
    }
    throw xuzy::LibraryUnLoadException(
        "Attempt to unload library that class_loader is unaware of.");
  }
}

}  // namespace impl
}  // namespace class_loader