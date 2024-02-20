#include "editor/panels/editor_main_panel.hpp"

namespace xuzy::Editor::Panels {

MainPanel::MainPanel(const std::string& p_name, bool p_opened)
    : PanelWindow(p_name, p_opened) {
  //   set_position({0.f, 0.f});
  resizable = false;
  movable = false;
  //   title_bar = false;
}

void MainPanel::_on_draw_impl() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {50.f, 50.f});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

  UI::Panels::PanelWindow::_on_draw_impl();

  ImGui::PopStyleVar(2);
}

}  // namespace xuzy::Editor::Panels