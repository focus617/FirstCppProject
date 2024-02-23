#pragma once

#include <vector>

#include "tools/event/event_dispatcher.hpp"
#include "tools/maths/FVector2.hpp"
#include "ui/widgets/layout/group.hpp"

namespace xuzy::UI::Widgets::Plots {

/**
 * @brief Widget that behave like a group with a plot display
 */
class Plot : public Layout::Group, public std::enable_shared_from_this<Plot> {
 public:
  /**
   * @brief Constructor
   * @param p_title
   * @param p_axis_x_label
   * @param p_axis_y_label
   */
  Plot(const std::string& p_title, 
       float p_axis_x_min, float p_axis_x_max,
       float p_axis_y_min, float p_axis_y_max,
       const std::string& p_axis_x_label = "",
       const std::string& p_axis_y_label = "",
       const Maths::FVector2& p_size = { -1.0f, -1.0f });

  std::shared_ptr<Plot> getptr() { return shared_from_this(); }

  void setup_axis_units(const std::string& p_axis_x_unit, 
                        const std::string& p_axis_y_unit);

 protected:
  virtual void _on_draw_impl() override;

 public:
  std::string title;
  float axis_x_min, axis_x_max;
  float axis_y_min, axis_y_max;
  std::string axis_x_label;
  std::string axis_y_label;
  char* axis_x_fmt = nullptr;
  char* axis_y_fmt = nullptr;
  std::string axis_x_unit = "";
  std::string axis_y_unit = "";

  Maths::FVector2 size = { -1.0f, -1.0f };

  Events::EventDispatcher<void()> event_dispatcher;
};
}  // namespace xuzy::UI::Widgets::Plots