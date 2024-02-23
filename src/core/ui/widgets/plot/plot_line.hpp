#pragma once

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Plots
{
	/**
	* @brief Plot displayed as lines
	*/
	class PlotLine : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_title
		* @param p_x_label
		* @param p_y_label
		*/
		PlotLine
		(
			const std::string& p_line_label,
			bool p_show_lines = true,
			bool p_show_fills = false
		);

		void enable_show_lines(bool p_value);

		void enable_show_fills(bool p_value);

	protected:
		void _on_draw_impl() override;
	
	private:
		std::string line_label;
		bool show_lines = true;
		bool show_fills = false;
		float fill_ref = 0;
		int shade_mode = 0;

		double xs[101], ys[101];
	};
}