#include "ui/widgets/text/text_selectable.hpp"

#include "tools/event/drawable_event.hpp"

namespace xuzy::UI::Widgets::Text {

TextSelectable::TextSelectable(const std::string& p_content, bool p_selected,
                               bool p_disabled)
    : Text(p_content), selected(p_selected), disabled(p_disabled) {}

void TextSelectable::_on_draw_impl() {
  if (ImGui::Selectable((content + m_widget_id).c_str(), &selected,
                        disabled ? ImGuiSelectableFlags_Disabled
                                 : ImGuiSelectableFlags_None)) {
    // Invoke ClickedEvent
    auto click_event = CreateRef<Events::DrawableClickedEvent>(
        Events::DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(click_event);

    if (selected) {
      // Invoke DrawableSelectedEvent
      auto select_event = CreateRef<Events::DrawableSelectedEvent>(
          Events::DrawableSelectedEvent(getptr()));
      event_dispatcher.dispatch(select_event);
    } else {
      // Invoke DrawableUnselectedEvent
      auto unselect_event = CreateRef<Events::DrawableUnselectedEvent>(
          Events::DrawableUnselectedEvent(getptr()));
      event_dispatcher.dispatch(unselect_event);
    }
  }
}

}  // namespace xuzy::UI::Widgets::Text
