#include "ui/widgets/plot/APlot.hpp"

xuzy::UI::Widgets::Plots::APlot::APlot(const std::string& p_title,
									   const std::vector<float>& p_data,
                                       float p_min_scale, float p_max_scale,
                                       const Maths::FVector2& p_size,
                                       const std::string& p_overlay,
                                       int p_force_hover)
    : DataWidget(data),
      title(p_title),
      data(p_data),
      min_scale(p_min_scale),
      max_scale(p_max_scale),
      size(p_size),
      overlay(p_overlay),
      force_hover(p_force_hover) {}