#pragma once

#include "ui/panels/APanel.hpp"

namespace xuzy::UI::Panels {

/**
 * @brief A simple panel that will be displayed on the top side of the canvas
 */
class PanelMenuBar : public APanel {
 protected:
  void _on_draw_impl() override;
};

}  // namespace xuzy::UI::Panels