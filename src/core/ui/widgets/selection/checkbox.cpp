#include "ui/widgets/selection/checkbox.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Selection::CheckBox::CheckBox(bool p_value,
                                                 const std::string& p_label)
    : DataWidget<bool>(value), value(p_value), label(p_label) {}

void xuzy::UI::Widgets::Selection::CheckBox::_on_draw_impl() {
  bool previous_value = value;

  ImGui::Checkbox((label + m_widget_id).c_str(), &value);

  if (value != previous_value) {
    // ValueChangedEvent.Invoke(value);
    auto event = CreateRef<Events::DrawableValueChangedEvent<bool>>(
          Events::DrawableValueChangedEvent<bool>(getptr(), value));
    event_dispatcher.dispatch(event);

	// Notify Plugins
    this->notify_change();
  }
}
