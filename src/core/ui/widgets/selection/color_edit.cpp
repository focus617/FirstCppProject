#include "ui/widgets/selection/color_edit.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Selection::ColorEdit::ColorEdit(
    bool p_enableAlpha, const Types::Color& p_defaultColor)
    : DataWidget<Types::Color>(color),
      enable_alpha(p_enableAlpha),
      color(p_defaultColor) {}

void xuzy::UI::Widgets::Selection::ColorEdit::_on_draw_impl() {
  int flags = !enable_alpha ? ImGuiColorEditFlags_NoAlpha : 0;
  bool value_changed = false;

  if (enable_alpha)
    value_changed = ImGui::ColorEdit4(m_widget_id.c_str(), &color.r, flags);
  else
    value_changed = ImGui::ColorEdit3(m_widget_id.c_str(), &color.r, flags);

  if (value_changed) {
    // ColorChangedEvent.Invoke(color);
    auto event = CreateRef<Events::DrawableValueChangedEvent<Types::Color>>(
        Events::DrawableValueChangedEvent<Types::Color>(getptr(), color));
    event_dispatcher.dispatch(event);

    // Notify Plugins
    this->notify_change();
  }
}
