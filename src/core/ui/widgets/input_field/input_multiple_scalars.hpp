#pragma once

#include <array>

#include "tools/event/drawable_event.hpp"
#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::InputField {

/**
 * @brief Input widget of multiple generic type
 */
template <typename T, size_t _Size>
class InputMultipleScalars
    : public DataWidget<std::array<T, _Size>>,
      public std::enable_shared_from_this<InputMultipleScalars<T, _Size>> {
  static_assert(_Size > 1,
                "Invalid InputMultipleScalars _Size (2 or more requiered)");
  static_assert(std::is_scalar<T>::value,
                "Invalid InputMultipleScalars T (Scalar expected)");

 public:
  /**
   * @brief Constructor
   * @param p_dataType
   * @param p_defaultValue
   * @param p_step
   * @param p_fastStep
   * @param p_label
   * @param p_format
   * @param p_selectAllOnClick
   */
  InputMultipleScalars(ImGuiDataType p_dataType, T p_defaultValue, T p_step,
                       T p_fastStep, const std::string& p_label,
                       const std::string& p_format, bool p_selectAllOnClick)
      : DataWidget<std::array<T, _Size>>(values),
        m_data_type(p_dataType),
        step(p_step),
        fastStep(p_fastStep),
        label(p_label),
        format(p_format),
        select_all_on_click(p_selectAllOnClick) {
    values.fill(p_defaultValue);
  }

  Ref<InputMultipleScalars<T, _Size>> getptr() {
    return this->shared_from_this();
  }

 protected:
  void _on_draw_impl() override {
    std::array<T, _Size> previous_value = values;

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (select_all_on_click) flags |= ImGuiInputTextFlags_AutoSelectAll;

    bool enterPressed = ImGui::InputScalarN(
        (label + this->m_widget_id).c_str(), m_data_type, values.data(), _Size,
        step != 0.0f ? &step : nullptr, fastStep != 0.0f ? &fastStep : nullptr,
        format.c_str(), flags);

    bool has_changed = false;

    for (size_t i = 0; i < _Size; ++i)
      if (previous_value[i] != values[i]) has_changed = true;

    if (has_changed) {
      // ContentChangedEvent.Invoke(values);
      auto event =
          CreateRef<Events::DrawableValueChangedEvent<std::array<T, _Size>>>(
              Events::DrawableValueChangedEvent<std::array<T, _Size>>(getptr(),
                                                                      values));
      event_dispatcher.dispatch(event);

      // Notify Plugins
      this->notify_change();
    }

    if (enterPressed) {
      // EnterPressedEvent.Invoke(values);
      auto event =
          CreateRef<Events::DrawableEnterPressedEvent<std::array<T, _Size>>>(
              Events::DrawableEnterPressedEvent<std::array<T, _Size>>(getptr(),
                                                                      values));
      event_dispatcher.dispatch(event);
    }
  }

 public:
  std::array<T, _Size> values;
  T step;
  T fastStep;
  std::string label;
  std::string format;
  bool select_all_on_click;

  Events::EventDispatcher<void()> event_dispatcher;

 private:
  ImGuiDataType m_data_type;
};
}  // namespace xuzy::UI::Widgets::InputField