#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Plots
{
	/**
	* @brief Plot displayed as an histogram
	*/
	class PlotHistogram : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_title
		*/
		PlotHistogram
		(
			const std::string& p_title = ""
		);

	protected:
		void _on_draw_impl() override;
	};
}