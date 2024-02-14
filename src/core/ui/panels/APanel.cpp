#include "APanel.hpp"

#include <algorithm>

namespace xuzy::UI::Panels {

uint64_t APanel::__PANEL_ID_INCREMENT = 0;

APanel::APanel() {
  m_panel_id = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void APanel::on_draw() {
  if (m_enabled) _on_draw_impl();
}

const std::string& APanel::get_panel_id() const {
  return m_panel_id;
}

}  // namespace xuzy::UI::Panels
