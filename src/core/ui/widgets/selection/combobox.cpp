#include "ui/widgets/selection/combobox.hpp"

#include <algorithm>

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::Selection::ComboBox::ComboBox(int p_current_choice)
    : DataWidget<int>(current_choice), current_choice(p_current_choice) {}

void xuzy::UI::Widgets::Selection::ComboBox::_on_draw_impl() {
  if (choices.find(current_choice) == choices.end())
    current_choice = choices.begin()->first;

  if (ImGui::BeginCombo(m_widget_id.c_str(), choices[current_choice].c_str())) {
    for (const auto& [key, value] : choices) {
      bool selected = (key == current_choice);

      if (ImGui::Selectable(value.c_str(), selected)) {
        if (!selected) {
          ImGui::SetItemDefaultFocus();
          current_choice = key;
          // ValueChangedEvent.Invoke(current_choice);
          auto event = CreateRef<Events::DrawableValueChangedEvent<int>>(
              Events::DrawableValueChangedEvent<int>(getptr(), current_choice));
          event_dispatcher.dispatch(event);

          // Notify Plugins
          this->notify_change();
        }
      }
    }

    ImGui::EndCombo();
  }
}
