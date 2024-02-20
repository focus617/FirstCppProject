#pragma once

#include <vector>
#include <limits>

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/AWidget.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Plots
{
	/**
	* @brief Base class for any plot widget
	*/
	class APlot : public DataWidget<std::vector<float>>
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
		APlot
		(
			const std::string& p_title = "",
			const std::vector<float>& p_data = std::vector<float>(),
			float p_min_scale = std::numeric_limits<float>::min(),
			float p_max_scale = std::numeric_limits<float>::max(),
			const Maths::FVector2& p_size = { 0.0f, 0.0f },
			const std::string& p_overlay = "",
			int p_force_hover = -1
		);

	protected:
		virtual void _on_draw_impl() override = 0;

	public:
		std::string title;
		std::vector<float> data;
		float min_scale;
		float max_scale;
		Maths::FVector2 size;
		std::string overlay;
		int force_hover;
	};
}