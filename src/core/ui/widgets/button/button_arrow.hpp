#pragma once

#include <string>

#include "ui/widgets/button/AButton.hpp"

namespace xuzy::UI::Widgets::Button {

/**
 * @brief Button widget with an arrow image
 */
class ButtonArrow : public AButton,
                    public std::enable_shared_from_this<ButtonArrow> {
 public:
  /**
   * @brief Create the button
   * @param p_direction
   */
  ButtonArrow(ImGuiDir p_direction = ImGuiDir_None);

  std::shared_ptr<ButtonArrow> getptr() { return shared_from_this(); }

 protected:
  void _on_draw_impl() override;

 public:
  ImGuiDir direction;
};

}  // namespace xuzy::UI::Widgets::Button