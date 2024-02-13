#pragma once

#include <algorithm>

#include "tools/event/event.hpp"
#include "ui/backend/opengl/imgui_include.h"
#include "ui/imgui/imgui.h"
#include "ui/panels/APanel.hpp"
#include "window/core/layer.hpp"

namespace xuzy::UI {
/**
 * @brief A Canvas represents the whole frame available for UI drawing
 */
class XUZY_API Canvas : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Create Canvas. Will draw each panel
   */
  Canvas() : ALayer("Canvas") {}

  virtual void on_attach() override { LOG(INFO) << "Canvas OnAttach"; }
  virtual void on_detach() override { LOG(INFO) << "Canvas OnDetach"; }
  virtual void on_update() override;
  virtual void on_event(Ref<Event> event, bool& handled) override;

  /**
   * @brief Draw the canvas (Draw every panels)
   */
  virtual void on_draw() override;

  /**
   * Adds a panel to the canvas
   * @param p_panel
   */
    void add_panel(Panels::APanel& p_panel);

  /**
   * @brief Removes a panel from the canvas
   * @param p_panel
   */
    void remove_panel(Panels::APanel& p_panel);

  /**
   * @brief Removes every panels from the canvas
   */
  void remove_all_panels();

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
  std::vector<std::reference_wrapper<Panels::APanel>> m_panels_;
};

}  // namespace xuzy::UI
