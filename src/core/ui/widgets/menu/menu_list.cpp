#include "ui/widgets/menu/menu_list.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui.h"

namespace xuzy::UI::Widgets::Menu {

MenuList::MenuList(const std::string& p_name, bool p_locked)
    : name(p_name), locked(p_locked) {}

void MenuList::_on_draw_impl() {
  if (ImGui::BeginMenu(name.c_str(), !locked)) {
    if (!m_opened) {
      // Invoke ClickedEvent
      auto event = CreateRef<Events::DrawableClickedEvent>(
          Events::DrawableClickedEvent(getptr()));
      event_dispatcher.dispatch(event);
      m_opened = true;
    }

    draw_widgets();
    ImGui::EndMenu();
  } else {
    m_opened = false;
  }
}

}  // namespace xuzy::UI::Widgets::Menu