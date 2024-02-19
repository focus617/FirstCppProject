#pragma once

#include "tools/event/event_dispatcher.hpp"
#include "ui/types/color.h"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Selection {
/**
 *  @brief Widget that allow the selection of a color with a color picker
 */
class ColorPicker : public DataWidget<Types::Color>,
                    public std::enable_shared_from_this<ColorPicker> {
 public:
  /**
   * @brief Constructor
   * @param p_enableAlpha
   * @param p_defaultColor
   */
  ColorPicker(bool p_enableAlpha = false,
              const Types::Color& p_defaultColor = {});

  Ref<ColorPicker> getptr() { return shared_from_this(); }

 protected:
  void _on_draw_impl() override;

 public:
  bool enable_alpha;
  Types::Color color;

  Events::EventDispatcher<void()> event_dispatcher;
};

}  // namespace xuzy::UI::Widgets::Selection