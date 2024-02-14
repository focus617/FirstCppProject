#pragma once

#include <algorithm>

#include "tools/event/event.hpp"
#include "ui/backend/opengl/imgui_include.h"
#include "ui/imgui/imgui.h"
#include "ui/panels/APanel.hpp"
#include "window/API/IDrawable.h"

namespace xuzy::UI {
/**
 * @brief A Canvas represents the whole frame available for UI drawing
 */
class XUZY_API Canvas : public xuzy::Window::API::IDrawable {
 public:
  /**
   * @brief Draw the canvas (Draw every panels)
   */
  virtual void on_draw() override;

  /**
   * @brief Makes the canvas a dockspace (Dockable panels will be able to
   * attach themselves to the canvas)
   */
  void make_dockspace(bool p_state);

  /**
   * @brief Returns true if the canvas is a dockspace
   */
  bool is_dockspace() const;

 private:
  bool m_is_dockspace_ = false;
};

}  // namespace xuzy::UI
