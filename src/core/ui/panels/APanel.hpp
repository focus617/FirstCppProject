#pragma once

#include <unordered_map>
#include <vector>

#include "tools/event/event.hpp"
#include "ui/internal/widget_container.hpp"
#include "window/API/IDrawable.h"
#include "window/core/time_step.hpp"

namespace xuzy::UI::Panels {

/**
 * @brief A Panel is a component of a canvas.
 * It is a sort of window in the UI
 */
class XUZY_API APanel : public xuzy::Window::API::IDrawable,
                        public xuzy::UI::Internal::WidgetContainer {
 public:
  /**
   * @brief Constructor
   */
  APanel();

  /**
   * @brief routine update on each ticket
   */
  virtual void on_update(Renderer::Times::Timestep p_ts) {}

  /**
   * @brief event handler
   */
  virtual void on_event(Ref<Events::Event> event, bool& handled) {}

  /**
   * @brief Draw the panel
   */
  void on_draw() override;

  /**
   * @brief Returns the panel identifier
   */
  const std::string& get_panel_id() const;

 protected:
  virtual void _on_draw_impl() = 0;

 public:
  bool m_enabled = true;

 protected:
  std::string m_panel_id;

 private:
  static uint64_t __PANEL_ID_INCREMENT;
};

}  // namespace xuzy::UI::Panels