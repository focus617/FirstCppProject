#include "ui/widgets/plot/plot_histogram.hpp"

#include "ui/implot/implot.h"
#include "ui/internal/converter.hpp"

xuzy::UI::Widgets::Plots::PlotHistogram::PlotHistogram
(
	const std::string& p_title,
	const std::vector<float>& p_data,
	float p_min_scale,
	float p_max_scale,
	const Maths::FVector2& p_size,
	const std::string& p_overlay,
	int p_force_hover
) : APlot(p_title, p_data, p_min_scale, p_max_scale, p_size, p_overlay, p_force_hover)
{
}

void xuzy::UI::Widgets::Plots::PlotHistogram::_on_draw_impl()
{
	// ImGui::PlotHistogram((label + m_widget_id).c_str(), data.data(), static_cast<int>(data.size()), 0, overlay.c_str(), min_scale, max_scale, Internal::Converter::ToImVec2(size), sizeof(float), force_hover);

	if (ImPlot::BeginPlot((title + m_widget_id).c_str())) {
		ImPlot::EndPlot();
	}
}
