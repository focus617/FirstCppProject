#include "editor/core/editor.hpp"

#include "editor/panels/editor_main_menubar.hpp"
#include "renderer/renderer.h"
#include "ui/core/panel_manager.hpp"
#include "ui/panels/panel_menubar.hpp"
#include "ui/panels/panel_window.hpp"
#include "ui/widgets/plot/plot.hpp"
#include "ui/widgets/plot/plot_line.hpp"
#include "ui/widgets/text/text_selectable.hpp"
#include "ui/widgets/text/text_labelled.hpp"

namespace xuzy::Editor {

Editor::Editor(const std::string& t_app_name, const std::string& t_version)
    : WindowApp{std::move(t_app_name), std::move(t_version)} {
  Ref<UI::PanelManager> panel_manager = CreateRef<UI::PanelManager>();

  push_layer(panel_manager);

  auto main_menu =
      panel_manager->CreatePanel<Panels::MainMenuBar>("MainMenuBar");

  auto main_panel =
      panel_manager->CreatePanel<UI::Panels::PanelWindow>("MainPanel", true);
  auto plot = main_panel->CreateWidget<UI::Widgets::Plots::Plot>(
      "Stock Prices", 0, 100, 100, 1600, "Days", "Price");
  plot->setup_axis_units("", "$");
  plot->CreateWidget<UI::Widgets::Plots::PlotLine>("XXStock1", true, true);
  plot->CreateWidget<UI::Widgets::Plots::PlotLine>("XXStock2", true, true);

  auto child_panel =
      panel_manager->CreatePanel<UI::Panels::PanelWindow>("ChildPanel", true);
  for (int i = 0; i < 100; i++) {
    // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
    char label[128];
    sprintf(label, "MyObject %d", i);
    child_panel->CreateWidget<UI::Widgets::Text::TextSelectable>(label);
  }

  auto stat_panel =
      panel_manager->CreatePanel<UI::Panels::PanelWindow>("Statistics", true);
  auto stats = Renderer::Renderer2D::get_stats();
  stat_panel->CreateWidget<UI::Widgets::Text::Text>("Renderer2D Stats:");
  stat_panel->CreateWidget<UI::Widgets::Text::TextLabelled>(
      std::to_string(stats.draw_calls), "Draw Calls");
}

}  // namespace xuzy::Editor