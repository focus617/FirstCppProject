#pragma once

#include "tools/event/event_dispatcher.hpp"
#include "ui/types/color.h"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Selection {
/**
 * @brief Widget that can open a color picker on click
 */
class ColorEdit : public DataWidget<Types::Color>,
                  public std::enable_shared_from_this<ColorEdit> {
 public:
  /**
   * @brief Constructor
   * @param p_enableAlpha
   * @param p_defaultColor
   */
  ColorEdit(bool p_enableAlpha = false,
            const Types::Color& p_defaultColor = {});

  Ref<ColorEdit> getptr() { return shared_from_this(); }

 protected:
  void _on_draw_impl() override;

 public:
  bool enable_alpha;
  Types::Color color;

  Events::EventDispatcher<void()> event_dispatcher;
};

}  // namespace xuzy::UI::Widgets::Selection