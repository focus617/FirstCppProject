#include "ui/widgets/selection/color_picker.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Selection::ColorPicker::ColorPicker(
    bool p_enable_alpha, const Types::Color& p_default_color)
    : DataWidget<Types::Color>(color),
      enable_alpha(p_enable_alpha),
      color(p_default_color) {}

void xuzy::UI::Widgets::Selection::ColorPicker::_on_draw_impl() {
  int flags = !enable_alpha ? ImGuiColorEditFlags_NoAlpha : 0;
  bool value_changed = false;

  if (enable_alpha)
    value_changed = ImGui::ColorPicker4(m_widget_id.c_str(), &color.r, flags);
  else
    value_changed = ImGui::ColorPicker3(m_widget_id.c_str(), &color.r, flags);

  if (value_changed) {
    // Invoke ColorChangedEvent.Invoke(color);
    auto event = CreateRef<Events::DrawableValueChangedEvent<Types::Color>>(
        Events::DrawableValueChangedEvent<Types::Color>(getptr(), color));
    event_dispatcher.dispatch(event);

    // Notify Plugins
    this->notify_change();
  }
}
