#include "ui/panels/panel_menubar.hpp"

#include "ui/imgui/imgui.h"
#include "ui/widgets/menu/menu_item.hpp"
#include "ui/widgets/menu/menu_list.hpp"

namespace xuzy::UI::Panels {

PanelMenuBar::PanelMenuBar() { create_file_menu(); }

void PanelMenuBar::_on_draw_impl() {
  if (!m_widgets.empty() && ImGui::BeginMainMenuBar()) {
    draw_widgets();
    ImGui::EndMainMenuBar();
  }
}

void PanelMenuBar::register_panel(const std::string& p_name,
                                  UI::Panels::PanelWindow& p_panel) {
  auto& menu_item = m_window_menu_->CreateWidget<UI::Widgets::Menu::MenuItem>(
      p_name, "", true, true);

  menu_item.event_dispatcher +=
      std::bind(&UI::Panels::PanelWindow::event_handler, &p_panel,
                std::placeholders::_1, std::placeholders::_2);

  m_panels_map_.emplace(p_name,
                        std::make_pair(std::ref(p_panel), std::ref(menu_item)));
}

void PanelMenuBar::create_file_menu() {
  auto& file_menu = CreateWidget<UI::Widgets::Menu::MenuList>("File");
  file_menu.CreateWidget<UI::Widgets::Menu::MenuItem>("New", "CTRL + N");
}

}  // namespace xuzy::UI::Panels