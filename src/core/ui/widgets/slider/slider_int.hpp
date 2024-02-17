#pragma once

#include "ui/widgets/slider/slider_single_scalar.hpp"

namespace xuzy::UI::Widgets::Sliders
{
	/**
	* @brief Slider widget of type int
	*/
	class SliderInt : public SliderSingleScalar<int>
	{
	public:
		/**
		* @brief Constructor
		* @param p_min
		* @param p_max
		* @param p_value
		* @param p_orientation
		* @param p_label
		* @param p_format
		*/
		SliderInt
		(
			int p_min = 0,
			int p_max = 100,
			int p_value = 50,
			ESliderOrientation p_orientation = ESliderOrientation::HORIZONTAL,
			const std::string& p_label = "",
			const std::string& p_format = "%d"
		);
	};
}