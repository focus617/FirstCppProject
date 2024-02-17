#include "ui/widgets/button/button_arrow.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Button::ButtonArrow::ButtonArrow(ImGuiDir p_direction)
    : direction(p_direction) {}

void xuzy::UI::Widgets::Button::ButtonArrow::_on_draw_impl() {
  if (ImGui::ArrowButton(m_widget_id.c_str(), direction)) {
    // Invoke ClickedEvent
    auto event = CreateRef<Events::DrawableClickedEvent>(
        Events::DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);
  }
}
