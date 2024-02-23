#pragma once

#include <classloader/class_loader.hpp>

namespace plugin {

/**
 * @def PLUGINLIB_EXPORT_CLASS(class_type, base_class_type)
 * Register a class with class loader to effectively export it for plugin
 * loading later.
 * @param class_type The real class name with namespace qualifier
 * (e.g.Animals::Lion)
 * @param base_class_type The real base class type from which class_type
 * inherits
 */
#define PLUGINLIB_EXPORT_CLASS(class_type, base_class_type) \
  CLASS_LOADER_REGISTER_CLASS(class_type, base_class_type)

}  // namespace plugin