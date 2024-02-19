#include "ui/widgets/selection/radio_button.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Selection::RadioButton::RadioButton(
    bool p_selected, const std::string& p_label)
    : DataWidget<bool>(m_selected), label(p_label) {
  if (p_selected) select();
}

void xuzy::UI::Widgets::Selection::RadioButton::select() {
  m_selected = true;
  // ClickedEvent.Invoke(m_radio_id);
  auto event = CreateRef<Events::DrawableClickedEvent>(
      Events::DrawableClickedEvent(getptr()));
  event_dispatcher.dispatch(event);
}

bool xuzy::UI::Widgets::Selection::RadioButton::is_selected() const {
  return m_selected;
}

bool xuzy::UI::Widgets::Selection::RadioButton::get_radio_id() const {
  return m_radio_id;
}

void xuzy::UI::Widgets::Selection::RadioButton::_on_draw_impl() {
  if (ImGui::RadioButton((label + m_widget_id).c_str(), m_selected)) {
    // ClickedEvent.Invoke(m_radio_id);
    auto event = CreateRef<Events::DrawableClickedEvent>(
        Events::DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);

    // Notify Plugins
    this->notify_change();
  }
}
