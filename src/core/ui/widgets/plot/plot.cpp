#include "ui/widgets/plot/plot.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/implot/implot.h"
#include "ui/internal/converter.hpp"

extern int LabelFormatter(double value, char* buff, int size, void* data);

namespace xuzy::UI::Widgets::Plots {

static bool show_lines = true;
static bool show_fills = true;
static float fill_ref = 0;
static int shade_mode = 0;

Plot::Plot(const std::string& p_title, float p_axis_x_min, float p_axis_x_max,
           float p_axis_y_min, float p_axis_y_max,
           const std::string& p_axis_x_label, const std::string& p_axis_y_label,
           const Maths::FVector2& p_size)
    : title(p_title),
      axis_x_min(p_axis_x_min),
      axis_x_max(p_axis_x_max),
      axis_y_min(p_axis_y_min),
      axis_y_max(p_axis_y_max),
      axis_x_label(p_axis_x_label),
      axis_y_label(p_axis_y_label),
      size(p_size) {}

void Plot::setup_axis_units(const std::string& p_axis_x_unit,
                            const std::string& p_axis_y_unit) {
  axis_x_unit = p_axis_x_unit;
  axis_y_unit = p_axis_y_unit;
}

void Plot::_on_draw_impl() {
  ImGui::Checkbox("Lines", &show_lines);
  ImGui::SameLine();
  ImGui::Checkbox("Fills", &show_fills);
  if (show_fills) {
    ImGui::SameLine();
    if (ImGui::RadioButton("To -INF", shade_mode == 0)) shade_mode = 0;
    ImGui::SameLine();
    if (ImGui::RadioButton("To +INF", shade_mode == 1)) shade_mode = 1;
    ImGui::SameLine();
    if (ImGui::RadioButton("To Ref", shade_mode == 2)) shade_mode = 2;
    if (shade_mode == 2) {
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      ImGui::DragFloat("##Ref", &fill_ref, 1, -100, 500);
    }
  }

  if (ImPlot::BeginPlot((title + m_widget_id).c_str(),
                        Internal::Converter::ToImVec2(size))) {
    ImPlot::SetupAxis(ImAxis_X1, axis_x_label.c_str());
    ImPlot::SetupAxis(ImAxis_Y1, axis_y_label.c_str());

    if (axis_x_fmt) {
      ImPlot::SetupAxisFormat(ImAxis_X1, axis_x_fmt);
    } else {
      ImPlot::SetupAxisFormat(ImAxis_X1, LabelFormatter,
                              (void*)axis_x_unit.c_str());
    }

    if (axis_y_fmt) {
      ImPlot::SetupAxisFormat(ImAxis_Y1, axis_y_fmt);
    } else {
      ImPlot::SetupAxisFormat(ImAxis_Y1, LabelFormatter,
                              (void*)axis_y_unit.c_str());
    }

    ImPlot::SetupAxesLimits(axis_x_min, axis_x_max, axis_y_min, axis_y_max);
    ImPlot::SetupLegend(ImPlotLocation_North);
    ImPlot::SetupFinish();

    draw_widgets();

    ImPlot::EndPlot();
  }
}

}  // namespace xuzy::UI::Widgets::Plots
