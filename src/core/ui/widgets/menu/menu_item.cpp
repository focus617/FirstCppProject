#include "ui/widgets/menu/menu_item.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui.h"

namespace xuzy::UI::Widgets::Menu {

MenuItem::MenuItem(const std::string& p_name, const std::string& p_shortcut,
                   bool p_checkable, bool p_checked)
    : DataWidget(selected),
      name(p_name),
      shortcut(p_shortcut),
      checkable(p_checkable),
      checked(p_checked) {}

void MenuItem::_on_draw_impl() {
  bool previousValue = checked;

  if (ImGui::MenuItem((name + m_widget_id).c_str(), shortcut.c_str(),
                      checkable ? &checked : nullptr, enabled)) {
    // Invoke ClickedEvent
    auto event =
        CreateRef<DrawableClickedEvent>(DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);
  }

  if (checked != previousValue) {
    // Invoke ValueChangedEvent.Invoke(checked);
    auto event = CreateRef<DrawableValueChangedEvent>(
        DrawableValueChangedEvent(getptr(), checked));
    event_dispatcher.dispatch(event);
	
    // Notify plugins
    this->notify_change();
  }
}

}  // namespace xuzy::UI::Widgets::Menu
