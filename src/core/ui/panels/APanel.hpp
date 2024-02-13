#pragma once

#include <unordered_map>
#include <vector>

#include "window/API/IDrawable.h"
#include "ui/internal/widget_container.hpp"

namespace xuzy::UI::Panels {

/**
 * @brief A Panel is a component of a canvas.
 * It is a sort of window in the UI
 */
class APanel : public xuzy::Window::API::IDrawable,
               public xuzy::UI::Internal::WidgetContainer {
 public:
  /**
   * @brief Constructor
   */
  APanel();

  /**
   * @brief Draw the panel
   */
  void on_draw() override;

  /**
   * @brief Returns the panel identifier
   */
  const std::string& get_panel_id() const;

 protected:
  virtual void _draw_impl() = 0;

 public:
  bool m_enabled = true;

 protected:
  std::string m_panel_id;

 private:
  static uint64_t __PANEL_ID_INCREMENT;
};

}  // namespace xuzy::UI::Panels