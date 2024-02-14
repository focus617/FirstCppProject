#pragma once

#include "ui/panels/APanel.hpp"
#include "ui/panels/panel_window.hpp"
#include "ui/widgets/menu/menu_item.hpp"
#include "ui/widgets/menu/menu_list.hpp"

namespace xuzy::UI::Panels {

/**
 * @brief A simple panel that will be displayed on the top side of the canvas
 */
class PanelMenuBar : public APanel {
  using PanelMap = std::unordered_map<
      std::string,
      std::pair<std::reference_wrapper<UI::Panels::PanelWindow>,
                std::reference_wrapper<UI::Widgets::Menu::MenuItem>>>;

 public:
  /**
   * @brief Constructor
   */
  PanelMenuBar();

  /**
   * @brief Register a panel to the menu bar window menu
   */
  void register_panel(const std::string& p_name,
                      UI::Panels::PanelWindow& p_panel);

 protected:
  void _on_draw_impl() override;

 private:
  void create_file_menu();

 private:
  PanelMap m_panels_map_;

  UI::Widgets::Menu::MenuList* m_window_menu_ = nullptr;
};

}  // namespace xuzy::UI::Panels