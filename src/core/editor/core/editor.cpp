#include "editor/core/editor.hpp"

#include "editor/panels/editor_main_menubar.hpp"
#include "editor/panels/renderer2D_panel.hpp"
#include "renderer/renderer.h"
#include "ui/panels/panel_menubar.hpp"
#include "ui/panels/panel_window.hpp"
#include "ui/widgets/plot/plot.hpp"
#include "ui/widgets/plot/plot_line.hpp"
#include "ui/widgets/text/text.hpp"
#include "ui/widgets/text/text_selectable.hpp"
#include "ui/widgets/visual/image.hpp"

namespace xuzy::Editor {

Editor::Editor(const std::string& t_app_name, const std::string& t_version)
    : WindowApp{std::move(t_app_name), std::move(t_version)} {
  m_panel_manager = CreateRef<UI::PanelManager>();

  push_layer(m_panel_manager);

  auto main_menu =
      m_panel_manager->CreatePanel<Panels::MainMenuBar>("MainMenuBar");

  auto main_panel =
      m_panel_manager->CreatePanel<UI::Panels::PanelWindow>("MainPanel", true);
  auto plot = main_panel->CreateWidget<UI::Widgets::Plots::Plot>(
      "Stock Prices", 0, 100, 100, 1600, "Days", "Price");
  plot->setup_axis_units("", "$");
  plot->CreateWidget<UI::Widgets::Plots::PlotLine>("XXStock1", true, true);
  plot->CreateWidget<UI::Widgets::Plots::PlotLine>("XXStock2", true, true);

  auto child_panel =
      m_panel_manager->CreatePanel<UI::Panels::PanelWindow>("ChildPanel", true);
  for (int i = 0; i < 100; i++) {
    // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
    char label[128];
    sprintf(label, "MyObject %d", i);
    child_panel->CreateWidget<UI::Widgets::Text::TextSelectable>(label);
  }

  auto stat_panel =
      m_panel_manager->CreatePanel<UI::Panels::PanelWindow>("Statistics", true);
  auto stats = Renderer::Renderer2D::get_stats();
  stat_panel->CreateWidget<UI::Widgets::Text::Text>("Renderer2D Stats:");
  stat_panel->CreateWidget<UI::Widgets::Text::Text>(
      "Draw Calls: " + std::to_string(stats.draw_calls));

  auto opengl_panel =
      m_panel_manager->CreatePanel<UI::Panels::Renderer2DPanel>("OpenGL", true);
  opengl_panel->CreateWidget<UI::Widgets::Visual::Image>(
      opengl_panel->get_framebuffer()->get_color_attachment_renderer_id(),
      Maths::FVector2(
          opengl_panel->get_framebuffer()->get_specification().width,
          opengl_panel->get_framebuffer()->get_specification().height));
}

}  // namespace xuzy::Editor