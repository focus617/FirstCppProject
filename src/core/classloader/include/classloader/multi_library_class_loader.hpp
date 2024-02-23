
#pragma once

#include <map>
#include <string>
#include <thread>
#include <vector>
// #include <cstddef>

#include <glog/logging.h>

#include "classloader/class_loader.hpp"
#include "core/exception.hpp"
#include "core/visibility_control.hpp"

namespace xuzy {
XUZY_DECLARE_EXCEPTION(XUZY_API, NoClassLoaderExistsException, RuntimeException)
}

namespace class_loader {

typedef std::string LibraryPath;
typedef std::map<LibraryPath, class_loader::ClassLoader*>
    LibraryToClassLoaderMap;
typedef std::vector<ClassLoader*> ClassLoaderVector;

/**
 * @class MultiLibraryClassLoader
 * @brief A ClassLoader that can bind more than one runtime library
 */
class XUZY_API MultiLibraryClassLoader {
 public:
  /**
   * @brief Constructor for the class
   * @param enable_ondemand_loadunload - Flag indicates if classes are to be
   * loaded/unloaded automatically as class_loader are created and destroyed
   */
  XUZY_API
  explicit MultiLibraryClassLoader(bool enable_ondemand_loadunload);

  /**
   * @brief Virtual destructor for class
   */
  XUZY_API
  virtual ~MultiLibraryClassLoader();

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base This version does not look in a specific library for the
   * factory, but rather the first open library that defines the classs
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to
   * instantiate
   * @return A std::shared_ptr<Base> to newly created plugin
   */
  template <class Base>
  std::shared_ptr<Base> createInstance(const std::string& class_name) {
    LOG(INFO) << "class_loader::MultiLibraryClassLoader: "
              << "Attempting to create instance of class type " << class_name
              << ".";

    ClassLoader* loader = getClassLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw xuzy::CreateClassException(
          "MultiLibraryClassLoader: Could not create object of class type " +
          class_name +
          " as no factory exists for it. Make sure that the library exists and "
          "was explicitly loaded through " +
          "MultiLibraryClassLoader::loadLibrary()");
    }

    return loader->createInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base This version takes a specific library to make explicit the
   * factory being used
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to
   * instantiate
   * @param library_path - the library from which we want to create the plugin
   * @return A std::shared_ptr<Base> to newly created plugin
   */
  template <class Base>
  std::shared_ptr<Base> createInstance(const std::string& class_name,
                                       const std::string& library_path) {
    ClassLoader* loader = getClassLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw xuzy::NoClassLoaderExistsException(
          "Could not create instance as there is no ClassLoader in "
          "MultiLibraryClassLoader bound to library " +
          library_path +
          " Ensure you called MultiLibraryClassLoader::loadLibrary()");
    }
    return loader->createInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base Same as createSharedInstance() except it returns a
   * std::unique_ptr.
   */
  template <class Base>
  ClassLoader::UniquePtr<Base> createUniqueInstance(
      const std::string& class_name) {
    LOG(INFO) << "class_loader::MultiLibraryClassLoader: "
              << "Attempting to create instance of class type " << class_name
              << ".";

    ClassLoader* loader = getClassLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw xuzy::CreateClassException(
          "MultiLibraryClassLoader: Could not create object of class type " +
          class_name +
          " as no factory exists for it. Make sure that the library exists and "
          "was explicitly loaded through " +
          "MultiLibraryClassLoader::loadLibrary()");
    }
    return loader->createUniqueInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base Same as createSharedInstance() except it returns a
   * std::unique_ptr.
   */
  template <class Base>
  ClassLoader::UniquePtr<Base> createUniqueInstance(
      const std::string& class_name, const std::string& library_path) {
    ClassLoader* loader = getClassLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw xuzy::NoClassLoaderExistsException(
          "Could not create instance as there is no ClassLoader in "
          "MultiLibraryClassLoader bound to library " +
          library_path +
          " Ensure you called MultiLibraryClassLoader::loadLibrary()");
    }
    return loader->createUniqueInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base This version does not look in a specific library for the
   * factory, but rather the first open library that defines the classs This
   * version should not be used as the plugin system cannot do automated safe
   * loading/unloadings
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the concrete plugin class we want to
   * instantiate
   * @return An unmanaged Base* to newly created plugin
   */
  template <class Base>
  Base* createUnmanagedInstance(const std::string& class_name) {
    ClassLoader* loader = getClassLoaderForClass<Base>(class_name);
    if (nullptr == loader) {
      throw xuzy::CreateClassException(
          "MultiLibraryClassLoader: Could not create class of type " +
          class_name);
    }
    return loader->createUnmanagedInstance<Base>(class_name);
  }

  /**
   * @brief Creates an instance of an object of given class name with ancestor
   * class Base This version takes a specific library to make explicit the
   * factory being used This version should not be used as the plugin system
   * cannot do automated safe loading/unloadings
   * @param Base - polymorphic type indicating Base class
   * @param class_name - name of class for which we want to create instance
   * @param library_path - the fully qualified path to the runtime library
   */
  template <class Base>
  Base* createUnmanagedInstance(const std::string& class_name,
                                const std::string& library_path) {
    ClassLoader* loader = getClassLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw xuzy::NoClassLoaderExistsException(
          "Could not create instance as there is no ClassLoader in "
          "MultiLibraryClassLoader bound to library " +
          library_path +
          " Ensure you called MultiLibraryClassLoader::loadLibrary()");
    }
    return loader->createUnmanagedInstance<Base>(class_name);
  }

  /**
   * @brief Indicates if a class has been loaded and can be instantiated
   * @param Base - polymorphic type indicating Base class
   * @param class_name - name of class that is be inquired about
   * @return true if loaded, false otherwise
   */
  template <class Base>
  bool isClassAvailable(const std::string& class_name) {
    std::vector<std::string> available_classes = getAvailableClasses<Base>();
    return available_classes.end() != std::find(available_classes.begin(),
                                                available_classes.end(),
                                                class_name);
  }

  /**
   * @brief Indicates if a library has been loaded into memory
   * @param library_path - The full qualified path to the runtime library
   * @return true if library is loaded, false otherwise
   */
  XUZY_API
  bool isLibraryAvailable(const std::string& library_path);

  /**
   * @brief Gets a list of all classes that are loaded by the class loader
   * @param Base - polymorphic type indicating Base class
   * @return A vector<string> of the available classes
   */
  template <class Base>
  std::vector<std::string> getAvailableClasses() {
    std::vector<std::string> available_classes;
    for (auto& loader : getAllAvailableClassLoaders()) {
      std::vector<std::string> loader_classes =
          loader->getAvailableClasses<Base>();
      available_classes.insert(available_classes.end(), loader_classes.begin(),
                               loader_classes.end());
    }
    return available_classes;
  }

  /**
   * @brief Gets a list of all classes loaded for a particular library
   * @param Base - polymorphic type indicating Base class
   * @return A vector<string> of the available classes in the passed library
   */
  template <class Base>
  std::vector<std::string> getAvailableClassesForLibrary(
      const std::string& library_path) {
    ClassLoader* loader = getClassLoaderForLibrary(library_path);
    if (nullptr == loader) {
      throw xuzy::NoClassLoaderExistsException(
          "There is no ClassLoader in MultiLibraryClassLoader bound to "
          "library " +
          library_path +
          " Ensure you called MultiLibraryClassLoader::loadLibrary()");
    }
    return loader->getAvailableClasses<Base>();
  }

  /**
   * @brief Gets a list of all libraries opened by this class loader
   @ @return A list of libraries opened by this class loader
   */
  XUZY_API
  std::vector<std::string> getRegisteredLibraries();

  /**
   * @brief Loads a library into memory for this class loader
   * @param library_path - the fully qualified path to the runtime library
   */
  XUZY_API
  void loadLibrary(const std::string& library_path);

  /**
   * @brief Unloads a library for this class loader
   * @param library_path - the fully qualified path to the runtime library
   */
  XUZY_API
  int unloadLibrary(const std::string& library_path);

 private:
  /**
   * @brief Indicates if on-demand (lazy) load/unload is enabled so libraries
   * are loaded/unloaded automatically as needed
   */
  bool isOnDemandLoadUnloadEnabled() { return enable_ondemand_loadunload_; }

  /**
   * @brief Gets a handle to the class loader corresponding to a specific
   * runtime library
   * @param library_path - the library from which we want to create the plugin
   * @return A pointer to the ClassLoader*, == nullptr if not found
   */
  ClassLoader* getClassLoaderForLibrary(const std::string& library_path);

  /**
   * @brief Gets a handle to the class loader corresponding to a specific class
   * @param class_name - name of class for which we want to create instance
   * @return A pointer to the ClassLoader*, == nullptr if not found
   */
  template <typename Base>
  ClassLoader* getClassLoaderForClass(const std::string& class_name) {
    ClassLoaderVector loaders = getAllAvailableClassLoaders();
    for (ClassLoaderVector::iterator i = loaders.begin(); i != loaders.end();
         ++i) {
      if (!(*i)->isLibraryLoaded()) {
        (*i)->loadLibrary();
      }
      if ((*i)->isClassAvailable<Base>(class_name)) {
        return *i;
      }
    }
    return nullptr;
  }

  /**
   * @brief Gets all class loaders loaded within scope
   */
  ClassLoaderVector getAllAvailableClassLoaders();

  /**
   * @brief Destroys all ClassLoaders
   */
  void shutdownAllClassLoaders();

 private:
  bool enable_ondemand_loadunload_;
  LibraryToClassLoaderMap active_class_loaders_;
  std::mutex loader_mutex_;
};

}  // namespace class_loader
