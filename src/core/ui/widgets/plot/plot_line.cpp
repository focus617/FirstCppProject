#include "ui/widgets/plot/plot_line.hpp"

#include <cmath>

#include "ui/implot/implot.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Widgets::Plots {

static ImPlotShadedFlags flags = 0;

template <typename T>
inline T RandomRange(T min, T max) {
  T scale = rand() / (T)RAND_MAX;
  return min + scale * (max - min);
}

PlotLine::PlotLine(const std::string& p_line_label, bool p_show_lines,
                     bool p_show_fills)
    : line_label(p_line_label),
      show_lines(p_show_lines),
      show_fills(p_show_fills) {
  srand(0);
  for (int i = 0; i < 101; ++i) {
    xs[i] = (float)i;
    ys[i] = RandomRange(200.0, 1500.0);
  }
}

void PlotLine::enable_show_lines(bool p_value) { show_lines = p_value; }

void PlotLine::enable_show_fills(bool p_value) { show_fills = p_value; }

void PlotLine::_on_draw_impl() {
  if (show_fills) {
    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);

    ImPlot::PlotShaded(line_label.c_str(), xs, ys, 101,
                       shade_mode == 0   ? -INFINITY
                       : shade_mode == 1 ? INFINITY
                                         : fill_ref,
                       flags);

    ImPlot::PopStyleVar();
  }

  if (show_lines) {
    ImPlot::PlotLine(line_label.c_str(), xs, ys, 101);
  }
}

}  // namespace xuzy::UI::Widgets::Plots
