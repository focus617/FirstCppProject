#pragma once

#include <vector>

#include "ui/plugins/IPlugin.h"

namespace xuzy::UI::Plugins {

/**
* @brief Inherit from this class to make your class "Pluginable"
* (Able to have plugins)
*/
class Pluginable {
 public:
  /**
   * @brief Destructor (Destroys every plugins)
   */
  ~Pluginable() { remove_all_plugins(); }

  /**
   * @brief Add a plugin
   * @param p_args
   */
  template <typename T, typename... Args>
  T& add_plugin(Args&&... p_args) {
    static_assert(std::is_base_of<Plugins::IPlugin, T>::value,
                  "T should derive from IPlugin");

    T* newPlugin = new T(std::forward<Args>(p_args)...);
    m_plugins.push_back(newPlugin);
    return *newPlugin;
  }

  /**
   * @brief Returns the plugin of the given type, or nullptr if not found
   */
  template <typename T>
  T* get_plugin() {
    static_assert(std::is_base_of<Plugins::IPlugin, T>::value,
                  "T should derive from IPlugin");

    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
      T* result = dynamic_cast<T*>(*it);
      if (result) return result;
    }

    return nullptr;
  }

  /**
   * @brief Execute every plugins
   */
  void execute_plugins() {
    for (auto& plugin : m_plugins) plugin->execute();
  }

  /**
   * @brief Remove every plugins
   */
  void remove_all_plugins() {
    for (auto& plugin : m_plugins) delete plugin;

    m_plugins.clear();
  }

 private:
  std::vector<IPlugin*> m_plugins;
};

}  // namespace xuzy::UI::Plugins