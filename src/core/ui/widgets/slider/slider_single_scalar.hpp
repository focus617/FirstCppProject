#pragma once

#include "tools/event/drawable_event.hpp"
#include "tools/event/event_dispatcher.hpp"
#include "tools/maths/FVector2.hpp"
#include "ui/internal/converter.hpp"
#include "ui/widgets/data_widget.hpp"
#include "ui/widgets/slider/slider_orientation.hpp"

namespace xuzy::UI::Widgets::Sliders {
/**
 * @brief Slider widget of generic type
 */
template <typename T>
class SliderSingleScalar
    : public DataWidget<T>,
      public std::enable_shared_from_this<SliderSingleScalar<T>> {
  static_assert(std::is_scalar<T>::value,
                "Invalid SliderSingleScalar T (Scalar expected)");

 public:
  /**
   * @brief Constructor
   * @param p_dataType
   * @param p_min
   * @param p_max
   * @param p_value
   * @param p_orientation
   * @param p_label
   * @param p_format
   */
  SliderSingleScalar(ImGuiDataType p_dataType, T p_min, T p_max, T p_value,
                     ESliderOrientation p_orientation,
                     const std::string& p_label, const std::string& p_format)
      : DataWidget<T>(value),
        min(p_min),
        max(p_max),
        value(p_value),
        orientation(p_orientation),
        label(p_label),
        format(p_format),
        m_data_type(p_dataType) {}

  Ref<SliderSingleScalar<T>> getptr() {
    return std::enable_shared_from_this<
        SliderSingleScalar<T>>::shared_from_this();
  }

 protected:
  void _on_draw_impl() override {
    if (max < min) max = min;

    if (value < min)
      value = min;
    else if (value > max)
      value = max;

    bool valueChanged = false;

    switch (orientation) {
      case ESliderOrientation::HORIZONTAL:
        valueChanged =
            ImGui::SliderScalar((label + this->m_widget_id).c_str(), m_data_type,
                                &value, &min, &max, format.c_str());
        break;
      case ESliderOrientation::VERTICAL:
        valueChanged = ImGui::VSliderScalar(
            (label + this->m_widget_id).c_str(),
            Internal::Converter::ToImVec2(verticalModeSize), m_data_type, &value,
            &min, &max, format.c_str());
        break;
    }

    if (valueChanged) {
      // ValueChangedEvent.Invoke(value);
      auto event = CreateRef<Events::DrawableValueChangedEvent<T>>(
          Events::DrawableValueChangedEvent<T>(getptr(), value));
      event_dispatcher.dispatch(event);

      // Notify plugins
      this->notify_change();
    }
  }

 public:
  T min;
  T max;
  T value;
  ESliderOrientation orientation;
  Maths::FVector2
      verticalModeSize; /* Only applied with ESliderOrientation::VERTICAL */
  std::string label;
  std::string format;
  Events::EventDispatcher<void()> event_dispatcher;
  // OvTools::Eventing::Event<T> ValueChangedEvent;

 private:
  ImGuiDataType m_data_type;
};
}  // namespace xuzy::UI::Widgets::Sliders