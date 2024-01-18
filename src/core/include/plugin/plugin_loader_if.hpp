#pragma once

#include <string>
#include <vector>

#include "classloader/class_loader.hpp"
#include "visibility_control.hpp"

namespace plugin {

/**
 * @brief Pure virtual interface class of plugin::PluginLoader which is not
 * templated. This allows the writing of non-templated manager code which can
 * call all the administrative functions of ClassLoaders - everything except
 * createClassInstance(), createInstance() and createUnmanagedInstance().
 */
class XUZY_API PluginLoaderIf {
 public:
  /**
   * @brief Empty virtual destructor.
   */
  XUZY_API 
  virtual ~PluginLoaderIf() {}

  /**
   * @brief Return a list of all available plugin manifest paths.
   * @return A vector of strings corresponding to the paths of all available
   * plugin manifests
   */
  XUZY_API 
  virtual std::vector<std::string> getPluginXmlPaths() = 0;

  /**
   * @brief a list of all available classes for this ClassLoader's base class
   * type.
   * @return A vector of strings corresponding to the names of all available
   * classes
   */
  XUZY_API 
  virtual std::vector<std::string> getDeclaredClasses() = 0;

  /**
   * @brief Refresh the list of all available classes for this ClassLoader's
   * base class type.
   * @throws pluginlib::LibraryLoadException if package manifest cannot be found
   */
  XUZY_API 
  virtual void refreshDeclaredClasses() = 0;

  /**
   * @brief Strip the package name off of a lookup name.
   * @param lookup_name The name of the plugin
   * @return The name of the plugin stripped of the package name
   */
  XUZY_API 
  virtual std::string getName(const std::string& lookup_name) = 0;

  /**
   * @brief Check if the class associated with a plugin name is available to be
   * loaded.
   * @param lookup_name The name of the plugin
   * @return True if the plugin is available, false otherwise
   */
  XUZY_API 
  virtual bool isClassAvailable(const std::string& lookup_name) = 0;

  /**
   * @brief Given the lookup name of a class, return the type of the derived
   * class associated with it.
   * @param lookup_name The name of the class
   * @return The name of the associated derived class
   */
  XUZY_API 
  virtual std::string getClassType(const std::string& lookup_name) = 0;

  /**
   * @brief Given the lookup name of a class, return its description.
   * @param lookup_name The lookup name of the class
   * @return The description of the class
   */
  XUZY_API 
  virtual std::string getClassDescription(const std::string& lookup_name) = 0;

  /**
   * @brief Given the lookup name of a class, return the type of the associated
   * base class.
   * @return The type of the associated base class
   */
  XUZY_API 
  virtual std::string getBaseClassType() const = 0;

  /**
   * @brief Given the name of a class, return name of the containing package.
   * @param lookup_name The name of the class
   * @return The name of the containing package
   */
  XUZY_API 
  virtual std::string getClassPackage(const std::string& lookup_name) = 0;

  /**
   * @brief Given the name of a class, return the path of the associated plugin
   * manifest.
   * @param lookup_name The name of the class
   * @return The path of the associated plugin manifest
   */
  XUZY_API 
  virtual std::string getPluginManifestPath(const std::string& lookup_name) = 0;

  /**
   * @brief Check if a given class is currently loaded.
   * @param lookup_name The lookup name of the class to query
   * @return True if the class is loaded, false otherwise
   */
  XUZY_API 
  virtual bool isClassLoaded(const std::string& lookup_name) = 0;

  /**
   * @brief Attempt to load a class with a given name.
   * @param lookup_name The lookup name of the class to load
   * @throws pluginlib::LibraryLoadException if the library for the
   *   class cannot be loaded
   */
  XUZY_API 
  virtual void loadLibraryForClass(const std::string& lookup_name) = 0;

  /**
   * @brief Attempt to unload a class with a given name.
   * @param lookup_name The lookup name of the class to unload
   * @throws pluginlib::LibraryUnloadException if the library for the class
   *   cannot be unloaded
   * @return The number of pending unloads until the library is removed from
   * memory
   */
  XUZY_API 
  virtual int unloadLibraryForClass(const std::string& lookup_name) = 0;

  /**
   * @brief the libraries that are registered and can be loaded.
   * @return A vector of strings corresponding to the names of registered
   * libraries
   */
  XUZY_API 
  virtual std::vector<std::string> getRegisteredLibraries() = 0;

  /**
   * @brief Given the name of a class, return the path to its associated
   * library.
   * @param lookup_name The name of the class
   * @return The path to the associated library
   */
  XUZY_API 
  virtual std::string getClassLibraryPath(const std::string& lookup_name) = 0;
};

}  // namespace plugin