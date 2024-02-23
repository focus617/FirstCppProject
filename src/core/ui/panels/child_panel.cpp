#include "ui/panels/child_panel.hpp"

namespace xuzy::UI::Panels {

ChildPanel::ChildPanel(const std::string& p_name, bool p_opened)
    : PanelWindow(p_name, p_opened) {}

void ChildPanel::_on_draw_impl() {
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

  ImGui::BeginChild(
      name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0),
      ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX, window_flags);

  UI::Panels::PanelWindow::_on_draw_impl();

  ImGui::EndChild();
}

}  // namespace xuzy::UI::Panels