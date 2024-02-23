#pragma once

#include <string>
#include <map>
#include <vector>
#include <glog/logging.h>

#include "classloader/multi_library_class_loader.hpp"
#include "plugin/class_desc.hpp"
#include "plugin/plugin_loader_if.hpp"

namespace plugin {

template <typename T>
using UniquePtr = class_loader::ClassLoader::UniquePtr<T>;

/**
 * @brief A class to help manage and load classes.
 *
 */
template <class T>
class ClassLoader : public PluginLoaderIf {
 public:
  typedef typename std::map<std::string, ClassDesc>::iterator ClassMapIterator;

  /**
   * @brief
   * @param package The package containing the base class
   * @param base_class The type of the base class for classes to be loaded
   * @param attrib_name The attribute to search for in manifext.xml files,
   * defaults to "plugin"
   * @param plugin_xml_paths The list of paths of plugin.xml files, defaults to
   * be crawled via ros::package::getPlugins()
   * @throws pluginlib::ClassLoaderException if package manifest cannot be found
   */
  ClassLoader(
      std::string package, std::string base_class,
      std::string attrib_name = std::string("plugin"),
      std::vector<std::string> plugin_xml_paths = std::vector<std::string>());

  ~ClassLoader();

 private:
  std::vector<std::string> plugin_xml_paths_;
  // Map from library to class's descriptions described in XML.
  std::map<std::string, ClassDesc> classes_available_;
  std::string package_;
  std::string base_class_;
  std::string attrib_name_;
  class_loader::MultiLibraryClassLoader
      lowlevel_class_loader_;  // The underlying classload
};

}  // namespace plugin

// Note: The implementation of the methods is in a separate file for clarity.
#include "./plugin_loader_impl.hpp"