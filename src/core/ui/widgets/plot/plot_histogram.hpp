#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/plot/APlot.hpp"

namespace xuzy::UI::Widgets::Plots
{
	/**
	* @brief Plot displayed as an histogram
	*/
	class PlotHistogram : public APlot
	{
	public:
		/**
		* @brief Constructor
		* @param p_title,
		* @param p_data
		* @param p_min_scale
		* @param p_max_scale
		* @param p_size
		* @param p_overlay
		* @param p_forceHover
		*/
		PlotHistogram
		(
			const std::string& p_title = "",
			const std::vector<float>& p_data = std::vector<float>(),
			float p_min_scale = std::numeric_limits<float>::min(),
			float p_max_scale = std::numeric_limits<float>::max(),
			const Maths::FVector2& p_size = { 0.0f, 0.0f },
			const std::string& p_overlay = "",
			int p_forceHover = -1
		);

	protected:
		void _on_draw_impl() override;
	};
}