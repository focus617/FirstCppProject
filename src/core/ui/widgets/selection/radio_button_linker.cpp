#include "ui/widgets/selection/radio_button_linker.hpp"

#include <algorithm>

#include "tools/event/drawable_event.hpp"

namespace xuzy::UI::Widgets::Selection {

RadioButtonLinker::RadioButtonLinker() : DataWidget<int>(m_selected) {}

void RadioButtonLinker::link(RadioButton& p_radio_button) {
  p_radio_button.m_radio_id = m_available_radio_id++;

  auto listener_id = p_radio_button.event_dispatcher +=
      std::bind(&RadioButtonLinker::on_radio_button_clicked, this,
                std::placeholders::_1, std::placeholders::_2);

  if (p_radio_button.is_selected() && m_selected == -1)
    m_selected = p_radio_button.m_radio_id;

  m_radio_buttons.emplace_back(listener_id, std::ref(p_radio_button));
}

void RadioButtonLinker::unlink(RadioButton& p_radio_button) {
  auto it = std::find_if(
      m_radio_buttons.begin(), m_radio_buttons.end(),
      [&p_radio_button](
          std::pair<Events::HandlerID, std::reference_wrapper<RadioButton>>&
              p_pair) { return &p_pair.second.get() == &p_radio_button; });

  if (it != m_radio_buttons.end()) {
    it->second.get().event_dispatcher.unsubscribe(it->first);
    m_radio_buttons.erase(it);
  }
}

int RadioButtonLinker::get_selected() const { return m_selected; }

void RadioButtonLinker::_on_draw_impl() {
  // The RadioButtonLinker is special, it has nothing to display :)
}

void RadioButtonLinker::on_radio_button_clicked(Ref<Events::Event> evt,
                                                bool& handled) {
  auto event = std::static_pointer_cast<Events::DrawableClickedEvent>(evt);
  auto radio = std::static_pointer_cast<UI::Widgets::Selection::RadioButton>(
      event->m_publisher);
  auto radio_id = radio->get_radio_id();

  if (radio_id != m_selected) {
    m_selected = radio_id;

    // Update all radios in this list
    for (const auto& [listener, radio_button] : m_radio_buttons)
      radio_button.get().m_selected =
          (radio_button.get().m_radio_id == m_selected);

    // ValueChangedEvent.Invoke(m_selected);
    auto click_event = CreateRef<Events::DrawableValueChangedEvent<int>>(
        Events::DrawableValueChangedEvent<int>(getptr(), m_selected));
    event_dispatcher.dispatch(click_event);

    // Notify Plugins
    this->notify_change();

  }
}

}  // namespace xuzy::UI::Widgets::Selection
