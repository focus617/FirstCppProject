#include "ui/widgets/plot/plot_histogram.hpp"

#include "ui/implot/implot.h"
#include "ui/internal/converter.hpp"

xuzy::UI::Widgets::Plots::PlotHistogram::PlotHistogram(
    const std::string& p_title) {}

void xuzy::UI::Widgets::Plots::PlotHistogram::_on_draw_impl() {
  // ImGui::PlotHistogram((label + m_widget_id).c_str(), data.data(),
  // static_cast<int>(data.size()), 0, overlay.c_str(), min_scale, max_scale,
  // Internal::Converter::ToImVec2(size), sizeof(float), force_hover);

  if (ImPlot::BeginPlot((m_widget_id).c_str())) {
    ImPlot::EndPlot();
  }
}
