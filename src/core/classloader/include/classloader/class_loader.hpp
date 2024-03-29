#pragma once

#include <cassert>
#include <functional>
#include <mutex>

#include "classloader/class_loader_core.hpp"
#include "classloader/register_macro.hpp"
#include "core/exception.hpp"
#include "core/visibility_control.hpp"

namespace class_loader {
/**
 * @class ClassLoader
 * @brief This class allows loading and unloading of dynamically linked
 * libraries which contain class definitions from which objects can be
 * created/destroyed during runtime (i.e. class_loader). Libraries loaded by a
 * ClassLoader are only accessible within scope of that ClassLoader object.
 */
class XUZY_API ClassLoader {
 public:
  template <typename Base>
  using DeleterType = std::function<void(Base*)>;

  template <typename Base>
  using UniquePtr = std::unique_ptr<Base, DeleterType<Base>>;

  /**
   * @brief  Constructor for ClassLoader
   * @param library_path - The path of the runtime library to load
   * @param ondemand_load_unload - Indicates if on-demand (lazy)
   * If it's true, unloading/loading of libraries occurs as plugins are
   * created/destroyed; If it's false, libraries will be loaded immediately and
   * won't be unloaded until class loader is destroyed or force unload.
   */
  XUZY_API
  explicit ClassLoader(const std::string& library_path,
                       bool ondemand_load_unload = false);

  /**
   * @brief  Destructor for ClassLoader. All libraries opened by this
   * ClassLoader are unloaded automatically.
   */
  XUZY_API
  virtual ~ClassLoader();

  /**
   * @brief Gets the full-qualified path and name of the library associated with
   * this class loader
   */
  XUZY_API
  std::string getLibraryPath() { return library_path_; }

  /**
   * @brief Indicates if the library is to be loaded/unloaded on
   * demand...meaning that only to load a lib when the first plugin is created
   * and automatically shut it down when last active plugin is destroyed.
   */
  XUZY_API
  bool isOnDemandLoadUnloadEnabled() { return ondemand_load_unload_; }

  /**
   * @brief  Indicates if a library is loaded within the scope of this
   * ClassLoader. Note that the library may already be loaded internally through
   * another ClassLoader, but until loadLibrary() method is called, the
   * ClassLoader cannot create objects from said library. If we want to see if
   * the library has been opened by somebody else, @see
   * isLibraryLoadedByAnyClassloader()
   * @param  library_path The path to the library to load
   * @return true if library is loaded within this ClassLoader object's scope,
   * otherwise false
   */
  XUZY_API
  bool isLibraryLoaded();

  /**
   * @brief  Indicates if a library is loaded by some entity in the plugin
   * system (another ClassLoader), but not necessarily loaded by this
   * ClassLoader
   * @return true if library is loaded within the scope of the plugin system,
   * otherwise false
   */
  XUZY_API
  bool isLibraryLoadedByAnyClassloader();

  /**
   * @brief  Attempts to load a library on behalf of the ClassLoader.
   * If the library is already opened, this method has no effect.
   * If the library has been already opened by some other entity (i.e. another
   * ClassLoader or global interface), this object is given permissions to
   * access any plugin classes loaded by that other entity.
   * @param  library_path The path to the library to load
   */
  XUZY_API
  void loadLibrary();

  /**
   * @brief  Attempts to unload a library loaded within scope of the
   * ClassLoader.
   * If the library is not opened, this method has no effect.
   * If the library is opened by other another ClassLoader, the library will NOT
   * be unloaded internally -- however this ClassLoader will no longer be able
   * to instantiate class_loader bound to that library. If there are plugin
   * objects that exist in memory created by this classloader, a warning message
   * will appear and the library will not be unloaded. If loadLibrary() was
   * called multiple times (e.g. in the case of multiple threads or purposefully
   * in a single thread), the user is responsible for calling unloadLibrary()
   * the same number of times. The library will not be unloaded within the
   * context of this classloader until the number of unload calls matches the
   * number of loads.
   * @return The number of times more unloadLibrary() has to be called for it to
   * be unbound from this ClassLoader
   */
  XUZY_API
  int unloadLibrary();

  // Class

  /**
   * @brief  Generates an instance of loadable classes (i.e. class_loader).
   *
   * It is not necessary for the user to call loadLibrary() as it will be
   * invoked automatically if the library is not yet loaded (which typically
   * happens when in "On Demand Load/Unload" mode).
   *
   * @param  derived_class_name The name of the class we want to create, @see
   * getAvailableClasses()
   * @return A std::shared_ptr<Base> to newly created plugin object
   */
  template <class Base>
  std::shared_ptr<Base> createInstance(const std::string& derived_class_name) {
    return std::shared_ptr<Base>(
        createRawInstance<Base>(derived_class_name, true),
        std::bind(&ClassLoader::onPluginDeletion<Base>, this,
                  std::placeholders::_1));
  }

  /**
   * @brief  Generates an instance of loadable classes (i.e. class_loader).
   *
   * It is not necessary for the user to call loadLibrary() as it will be
   * invoked automatically if the library is not yet loaded (which typically
   * happens when in "On Demand Load/Unload" mode).
   *
   * If you release the wrapped pointer you must manually call the original
   * deleter when you want to destroy the released pointer.
   *
   * @param  derived_class_name The name of the class we want to create (@see
   * getAvailableClasses())
   * @return A std::unique_ptr<Base> to newly created plugin object
   */
  template <class Base>
  UniquePtr<Base> createUniqueInstance(const std::string& derived_class_name) {
    return std::unique_ptr<Base, DeleterType<Base>>(
        createRawInstance<Base>(derived_class_name, true),
        std::bind(&ClassLoader::onPluginDeletion<Base>, this,
                  std::placeholders::_1));
  }

  /**
   * @brief  Generates an instance of loadable classes (i.e. class_loader).
   *
   * It is not necessary for the user to call loadLibrary() as it will be
   * invoked automatically if the library is not yet loaded (which typically
   * happens when in "On Demand Load/Unload" mode).
   *
   * Creating an unmanaged instance disables dynamically unloading libraries
   * when managed pointers go out of scope for all class loaders in this
   * process.
   *
   * @param derived_class_name The name of the class we want to create (@see
   * getAvailableClasses())
   * @return An unmanaged (i.e. not a shared_ptr) Base* to newly created plugin
   * object.
   */
  template <class Base>
  Base* createUnmanagedInstance(const std::string& derived_class_name) {
    return createRawInstance<Base>(derived_class_name, false);
  }

  /**
   * @brief  Indicates which classes (i.e. class_loader) that can be loaded by
   * this object
   * @return vector of strings indicating names of instantiable classes derived
   * from <Base>
   */
  template <class Base>
  std::vector<std::string> getAvailableClasses() {
    return class_loader::impl::getAvailableClasses<Base>(this);
  }

  /**
   * @brief Indicates if a plugin class is available
   * @param Base - polymorphic type indicating base class
   * @param class_name - the name of the plugin class
   * @return true if yes it is available, false otherwise
   */
  template <class Base>
  bool isClassAvailable(const std::string& class_name) {
    std::vector<std::string> available_classes = getAvailableClasses<Base>();
    return std::find(available_classes.begin(), available_classes.end(),
                     class_name) != available_classes.end();
  }

 private:
  /**
   * @brief As the library may be unloaded in "on-demand load/unload" mode,
   * unload maybe called from createInstance(). The problem is that
   * createInstance() locks the plugin_ref_count as does unloadLibrary(). This
   * method is the implementation of unloadLibrary but with a parameter to
   * decide if plugin_ref_mutex_ should be locked
   * @param lock_plugin_ref_count - Set to true if plugin_ref_count_mutex_
   * should be locked, else false
   * @return The number of times unloadLibraryInternal has to be called again
   * for it to be unbound from this ClassLoader
   */
  XUZY_API
  int unloadLibraryInternal(bool lock_plugin_ref_count);

  /**
   * @brief Callback method when a plugin created by this class loader is
   * destroyed
   * @param obj - A pointer to the deleted object
   */
  template <class Base>
  void onPluginDeletion(Base* obj) {
    LOG(INFO) << "class_loader::ClassLoader: Calling onPluginDeletion() for "
                 "obj ptr = "
              << reinterpret_cast<void*>(obj) << ".\n";

    if (nullptr == obj) {
      return;
    }

    std::scoped_lock lock(plugin_ref_count_mutex_);
    delete (obj);

    plugin_ref_count_ = plugin_ref_count_ - 1;
    assert(plugin_ref_count_ >= 0);
    if (0 == plugin_ref_count_ && isOnDemandLoadUnloadEnabled()) {
      if (!ClassLoader::hasUnmanagedInstanceBeenCreated()) {
        unloadLibraryInternal(false);
      } else {
        LOG(WARNING) << "class_loader::ClassLoader: "
                     << "Cannot unload library " << getLibraryPath()
                     << " even though last shared pointer went out of scope."
                     << " This is because createUnmanagedInstance was used "
                        "within the scope of this process, perhaps by a "
                        "different ClassLoader. Library will NOT be closed.";
      }
    }
  }

  /**
   * @brief  Generates an instance of loadable classes (i.e. class_loader).
   *
   * It is not necessary for the user to call loadLibrary() as it will be
   * invoked automatically, if the library is not yet loaded (which typically
   * happens when in "On Demand Load/Unload" mode).
   *
   * @param  derived_class_name The name of the class we want to create (@see
   * getAvailableClasses())
   * @param  managed If true, the returned pointer is assumed to be wrapped in a
   * smart pointer by the caller.
   * @return A Base* to newly created plugin object
   */
  template <class Base>
  Base* createRawInstance(const std::string& derived_class_name, bool managed) {
    if (!managed) {
      has_unmananged_instance_been_created_ = true;
    }

    if (managed && ClassLoader::hasUnmanagedInstanceBeenCreated() &&
        isOnDemandLoadUnloadEnabled()) {
      LOG(INFO) << "class_loader::ClassLoader: "
                << "An attempt is being made to create a managed plugin "
                   "instance (i.e. boost::shared_ptr), however an unmanaged "
                   "instance was created within this process address space. "
                << "This means libraries for the managed instances will not be "
                   "shutdown automatically on final plugin destruction if on "
                   "demand (lazy) loading/unloading mode is used.";
    }
    if (!isLibraryLoaded()) {
      LOG(INFO) << "class_loader::ClassLoader: lazy load library.";
      loadLibrary();
    }

    Base* obj =
        class_loader::impl::createInstance<Base>(derived_class_name, this);
    // Unreachable assertion if createInstance() throws on failure
    assert(obj != nullptr);

    if (managed) {
      std::scoped_lock lock(plugin_ref_count_mutex_);
      ++plugin_ref_count_;
    }

    return obj;
  }

  /**
   * @brief Getter for if an unmanaged (i.e. unsafe) instance has been created
   * flag
   */
  XUZY_API
  static bool hasUnmanagedInstanceBeenCreated();

 private:
  bool ondemand_load_unload_;
  std::string library_path_;
  int load_ref_count_;
  std::recursive_mutex load_ref_count_mutex_;
  int plugin_ref_count_;
  std::recursive_mutex plugin_ref_count_mutex_;

  XUZY_API
  static bool has_unmananged_instance_been_created_;
};

}  // namespace class_loader