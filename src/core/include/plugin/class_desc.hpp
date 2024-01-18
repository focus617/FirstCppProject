#pragma once

#include <string>

namespace plugin
{

class ClassDesc
{
public:
  /**
   * @brief Storage for information about a given class.
   * @param lookup_name The lookup name of the class
   * @param derived_class The type of the derived class of the class
   * @param base_class The type of the class, corresponds to the type of the base class
   * @param package The package the class lives in
   * @param description A description for the class
   * @param library_name The name of the containing library for the class (not a full path!)
   * @param plugin_manifest_path The path to the plugin manifest file
   */
  ClassDesc(
    const std::string & lookup_name, const std::string & derived_class,
    const std::string & base_class, const std::string & package,
    const std::string & description, const std::string & library_name,
    const std::string & plugin_manifest_path)
  : lookup_name_(lookup_name),
    derived_class_(derived_class),
    base_class_(base_class),
    package_(package),
    description_(description),
    library_name_(library_name),
    resolved_library_path_("UNRESOLVED"),
    plugin_manifest_path_(plugin_manifest_path) {}

  std::string lookup_name_;
  std::string derived_class_;
  std::string base_class_;
  std::string package_;
  std::string description_;
  std::string library_name_;
  std::string resolved_library_path_;  // This is set by pluginlib::ClassLoader at load time.
  std::string plugin_manifest_path_;
};

}  // namespace plugin