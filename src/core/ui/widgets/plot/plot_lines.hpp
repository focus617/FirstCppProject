#pragma once

#include "ui/widgets/plot/APlot.hpp"

namespace xuzy::UI::Widgets::Plots
{
	/**
	* @brief Plot displayed as lines
	*/
	class PlotLines : public APlot
	{
	public:
		/**
		* @brief Constructor
		* @param p_title
		* @param p_data
		* @param p_min_scale
		* @param p_max_scale
		* @param p_size
		* @param p_overlay
		* @param p_force_hover
		*/
		PlotLines
		(
			const std::string& p_title = "",
			const std::vector<float>& p_data = std::vector<float>(),
			float p_min_scale = std::numeric_limits<float>::min(),
			float p_max_scale = std::numeric_limits<float>::max(),
			const Maths::FVector2& p_size = { 0.0f, 0.0f },
			const std::string& p_overlay = "",
			int p_force_hover = -1,
			const std::string& p_x_label = "",
			const std::string& p_y_label = ""
		);

	protected:
		void _on_draw_impl() override;
	
	public:
		std::string x_label;
		std::string y_label;
	};
}