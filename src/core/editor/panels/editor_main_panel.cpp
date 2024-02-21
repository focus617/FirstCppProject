#include "editor/panels/editor_main_panel.hpp"

#include "ui/widgets/text/text_wrapped.hpp"

namespace xuzy::Editor::Panels {

MainPanel::MainPanel(const std::string& p_name, bool p_opened)
    : PanelWindow(p_name, p_opened) {
//   set_position({0.f, 0.f});
//   resizable = false;
//   movable = false;
//   title_bar = false;

    auto text = CreateWidget<UI::Widgets::Text::TextWrapped>("Give someone state and they'll have a bug one day, but teach them how to represent state in two separate locations that have to be kept in sync and they'll have bugs for a lifetime.");
    text->set_tool_tip_desc("TextWrapped");
    text->enable_tool_tip(true);
}

void MainPanel::_on_draw_impl() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {50.f, 50.f});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

  UI::Panels::PanelWindow::_on_draw_impl();

  ImGui::PopStyleVar(2);

  ImGui::ShowDemoWindow();
}

}  // namespace xuzy::Editor::Panels