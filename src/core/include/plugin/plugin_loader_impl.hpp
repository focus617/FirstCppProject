#include <memory>

#include "classloader/class_loader.hpp"

#ifdef _WIN32
const std::string os_pathsep(";");  // NOLINT
#else
const std::string os_pathsep(":");  // NOLINT
#endif

namespace xuzy {
XUZY_DECLARE_EXCEPTION(XUZY_API, ClassLoaderException, RuntimeException)
}

namespace plugin {

template <class T>
ClassLoader<T>::ClassLoader(std::string package, std::string base_class,
                            std::string attrib_name,
                            std::vector<std::string> plugin_xml_paths)
    : plugin_xml_paths_(plugin_xml_paths),
      package_(package),
      base_class_(base_class),
      attrib_name_(attrib_name),
      // NOTE: The parameter to the class loader enables/disables on-demand
      // class loading/unloading. Leaving it off for now... libraries will be
      // loaded immediately and won't be unloaded until class loader is
      // destroyed or force unload.
      lowlevel_class_loader_(false) {
  LOG(INFO) << "pluginlib.ClassLoader"
            << "Creating ClassLoader, base = " << base_class
            << ", address = " << this;
  // if (ros::package::getPath(package_).empty()) {
  //   throw xuzy::ClassLoaderException("Unable to find package: " + package_);
  // }

  // if (0 == plugin_xml_paths_.size()) {
  //   plugin_xml_paths_ = getPluginXmlPaths(package_, attrib_name_);
  // }
  // classes_available_ = determineAvailableClasses(plugin_xml_paths_);
  LOG(INFO) << "pluginlib.ClassLoader"
            << "Finished constructring ClassLoader, base = " << base_class
            << ", address = " << this;
}

template <class T>
ClassLoader<T>::~ClassLoader() {
  LOG(INFO) << "pluginlib.ClassLoader"
            << "Destroying ClassLoader, base = " << base_class_
            << ", address = " << this;
}

}  // namespace plugin
