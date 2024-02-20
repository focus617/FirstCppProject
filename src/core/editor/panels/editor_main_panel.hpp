#pragma once

#include "ui/panels/panel_window.hpp"
#include "ui/settings/panel_window_settings.h"

namespace xuzy::Editor::Panels {

/**
 * @brief The main panel that will be displayed on the center of
 * the canvas
 */
class MainPanel : public UI::Panels::PanelWindow {
 public:
  /**
   * @brief Constructor
   */
  MainPanel(const std::string& p_name, bool p_opened = true);

 protected:
  virtual void _on_draw_impl() override;
};

}  // namespace xuzy::Editor::Panels