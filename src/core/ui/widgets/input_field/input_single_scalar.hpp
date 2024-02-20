#pragma once

#include "tools/event/drawable_event.hpp"
#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::InputField {
/**
 * @brief Input widget of generic type
 */
template <typename T>
class InputSingleScalar
    : public DataWidget<T>,
      public std::enable_shared_from_this<InputSingleScalar<T>> {
  static_assert(std::is_scalar<T>::value,
                "Invalid InputSingleScalar T (Scalar expected)");

 public:
  /**
   * @brief Constructor
   * @param p_data_type
   * @param p_default_value
   * @param p_step
   * @param p_fast_step
   * @param p_label
   * @param p_format
   * @param p_select_all_on_click
   */
  InputSingleScalar(ImGuiDataType p_data_type, T p_default_value, T p_step,
                    T p_fast_step, const std::string& p_label,
                    const std::string& p_format, bool p_select_all_on_click)
      : DataWidget<T>(value),
        value(p_default_value),
        step(p_step),
        fast_step(p_fast_step),
        label(p_label),
        format(p_format),
        select_all_on_click(p_select_all_on_click),
        m_data_type(p_data_type) {}

  Ref<InputSingleScalar<T>> getptr() { return this->shared_from_this(); }

 protected:
  void _on_draw_impl() override {
    T previousValue = value;

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (select_all_on_click) flags |= ImGuiInputTextFlags_AutoSelectAll;

    bool enterPressed = ImGui::InputScalar(
        (label + this->m_widget_id).c_str(), m_data_type, &value,
        step != 0.0f ? &step : nullptr,
        fast_step != 0.0f ? &fast_step : nullptr, format.c_str(), flags);

    if (previousValue != value) {
      // ContentChangedEvent.Invoke(value);
      auto event = CreateRef<Events::DrawableValueChangedEvent<T>>(
          Events::DrawableValueChangedEvent<T>(getptr(), value));
      event_dispatcher.dispatch(event);

      // Notify Plugins
      this->notify_change();
    }

    if (enterPressed) {
      // EnterPressedEvent.Invoke(value);
      auto event = CreateRef<Events::DrawableEnterPressedEvent<T>>(
          Events::DrawableEnterPressedEvent<T>(getptr(), value));
      event_dispatcher.dispatch(event);
    }
  }

 public:
  T value;
  T step;
  T fast_step;
  std::string label;
  std::string format;
  bool select_all_on_click;

  Events::EventDispatcher<void()> event_dispatcher;

 private:
  ImGuiDataType m_data_type;
};
}  // namespace xuzy::UI::Widgets::InputField