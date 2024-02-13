#pragma once

namespace xuzy::UI::Plugins {

/**
 * @brief Interface to any plugin of UI.
 * A plugin is basically a behaviour that you can plug to a widget
 */
class IPlugin {
 public:
  virtual ~IPlugin() = default;
  
  /**
   * @brief Execute the plugin behaviour
   */
  virtual void execute() = 0;

  /**
   *  @brief Feel free to store any data you want here
   */
  void* user_data = nullptr;

};

}  // namespace xuzy::UI::Plugins