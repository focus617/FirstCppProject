#pragma once

#include <string>

#include "tools/maths/FVector2.hpp"
#include "ui/types/color.h"
#include "ui/widgets/button/AButton.hpp"

namespace xuzy::UI::Widgets::Button {

/**
 * @brief Button widget of a single color (Color palette element)
 */
class ButtonColored : public AButton,
                      public std::enable_shared_from_this<ButtonColored> {
 public:
  /**
   * @brief Constructor
   * @param p_label
   * @param p_color
   * @param p_size
   * @param p_enableAlpha
   */
  ButtonColored(const std::string& p_label = "",
                const Types::Color& p_color = {},
                const Maths::FVector2& p_size = Maths::FVector2(0.f, 0.f),
                bool p_enableAlpha = true);

  std::shared_ptr<ButtonColored> getptr() { return shared_from_this(); }

 protected:
  void _on_draw_impl() override;

 public:
  std::string label;
  Types::Color color;
  Maths::FVector2 size;
  bool enableAlpha;
};

}  // namespace xuzy::UI::Widgets::Button