#pragma once

#include "ui/panels/panel_window.hpp"
#include "ui/settings/panel_window_settings.h"

namespace xuzy::UI::Panels {

/**
 * @brief The child panel which is a self-contained independent scrolling/
 * clipping regions within a host window panel.
 */
class ChildPanel : public UI::Panels::PanelWindow {
 public:
  /**
   * @brief Constructor
   */
  ChildPanel(const std::string& p_name, bool p_opened = true);

 protected:
  virtual void _on_draw_impl() override;
};

}  // namespace xuzy::UI::Panels