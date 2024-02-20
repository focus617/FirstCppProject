#pragma once

#include <OvTools/Eventing/Event.h>

#include <array>

#include "ui/widgets/DataWidget.h"

namespace xuzy::UI::Widgets::Sliders {
/**
 * @brief Slider widget of multiple generic type
 */
template <typename T, size_t _Size>
class SliderMultipleScalars : public DataWidget<std::array<T, _Size>> {
  static_assert(_Size > 1,
                "Invalid SliderMultipleScalars _Size (2 or more requiered)");

 public:
  /**
   * @brief Constructor
   * @param p_dataType
   * @param p_min
   * @param p_max
   * @param p_value
   * @param p_label
   * @param p_format
   */
  SliderMultipleScalars(ImGuiDataType_ p_dataType, T p_min, T p_max, T p_value,
                        const std::string& p_label, const std::string& p_format)
      : DataWidget<std::array<T, _Size>>(values),
        m_data_type(p_dataType),
        min(p_min),
        max(p_max),
        label(p_label),
        format(p_format) {
    values.fill(p_value);
  }

 protected:
  void _on_draw_impl() override {
    if (max < min) max = min;

    for (size_t i = 0; i < _Size; ++i) {
      if (values[i] < min)
        values[i] = min;
      else if (values[i] > max)
        values[i] = max;
    }

    bool valueChanged =
        ImGui::SliderScalarN((label + this->m_widget_id).c_str(), m_data_type,
                             values.data(), _Size, &min, &max, format.c_str());

    if (valueChanged) {
      ValueChangedEvent.Invoke(values);
      this->NotifyChange();
    }
  }

 public:
  T min;
  T max;
  std::array<T, _Size> values;
  std::string label;
  std::string format;
  OvTools::Eventing::Event<std::array<T, _Size>&> ValueChangedEvent;

 protected:
  ImGuiDataType_ m_data_type;
};
}  // namespace xuzy::UI::Widgets::Sliders