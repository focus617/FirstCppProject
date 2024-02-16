#pragma once

#include "ui/panels/panel_menubar.hpp"

namespace xuzy::Editor::Panels {

/**
 * @brief A simple panel that will be displayed on the top side of the canvas
 */
class MainMenuBar : public UI::Panels::PanelMenuBar {
 public:
  /**
   * @brief Constructor
   */
  MainMenuBar();

 private:
  void create_file_menu();
  void create_edit_menu();
  void create_help_menu();
};

}  // namespace xuzy::Editor::Panels 