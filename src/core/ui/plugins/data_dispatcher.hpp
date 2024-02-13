#pragma once

#include <functional>

#define TRY_GATHER(type, output)                                         \
  if (auto plugin = get_plugin<Plugins::DataDispatcher<type>>(); \
      plugin)                                                            \
    output = plugin->Gather();

#define TRY_PROVIDE(type, output)                                         \
  if (auto plugin = get_plugin<xuzy::UI::Plugins::DataDispatcher<type>>(); \
      plugin)                                                             \
    plugin->Provide(output);

#define TRY_NOTIFY_CHANGE(type)                                           \
  if (auto plugin = get_plugin<xuzy::UI::Plugins::DataDispatcher<type>>(); \
      plugin)                                                             \
    plugin->notify_change();

namespace xuzy::UI::Plugins {

/**
 * @brief Plugin that allow automatic data update of any DataWidget
 */
template <typename T>
class DataDispatcher : public IPlugin {
 public:
  /**
   * @brief Register a reference
   * @param p_reference
   */
  void RegisterReference(T& p_reference) { m_dataPointer = &p_reference; }

  /**
   * @brief Register a provider (Which function should be called when
   * the widget data is modified)
   * @param p_provider
   */
  void RegisterProvider(std::function<void(T)> p_provider) {
    m_provider = p_provider;
  }

  /**
   * @brief Register a gather (Which function should be called when the
   * widget data needs to be updated)
   * @param p_provider
   */
  void RegisterGatherer(std::function<T(void)> p_gatherer) {
    m_gatherer = p_gatherer;
  }

  /**
   * @brief Provide data to the dispatcher
   * @param p_data
   */
  void Provide(T p_data) {
    if (m_valueChanged) {
      if (m_dataPointer)
        *m_dataPointer = p_data;
      else
        m_provider(p_data);

      m_valueChanged = false;
    }
  }

  /**
   * @brief Notify that a change occured
   */
  void notify_change() { m_valueChanged = true; }

  /**
   * @brief Returns the data from the dispatcher
   */
  T Gather() { return m_dataPointer ? *m_dataPointer : m_gatherer(); }

  /**
   * @brief Execute the data dispatcher behaviour (No effect)
   */
  virtual void execute() override {}

 private:
  bool m_valueChanged = false;
  T* m_dataPointer = nullptr;
  std::function<void(T)> m_provider;
  std::function<T(void)> m_gatherer;
};

}  // namespace xuzy::UI::Plugins