#pragma once

#include "ui/widgets/slider/SliderMultipleScalars.h"

namespace xuzy::UI::Widgets::Sliders
{
	/**
	* @brief Slider widget of multiple type int
	*/
	template <size_t _Size>
	class SliderMultipleInts : public SliderMultipleScalars<int, _Size>
	{
	public:
		/**
		* @brief Constructor
		* @param p_min
		* @param p_max
		* @param p_value
		* @param p_label
		* @param p_format
		*/
		SliderMultipleInts
		(
			int p_min = 0,
			int p_max = 100,
			int p_value = 50,
			const std::string& p_label = "",
			const std::string& p_format = "%d"
		) : SliderMultipleScalars<int, _Size>(ImGuiDataType_::ImGuiDataType_S32, p_min, p_max, p_value, p_label, p_format) {}
	};
}