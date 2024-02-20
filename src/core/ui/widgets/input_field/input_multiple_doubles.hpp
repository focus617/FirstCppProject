#pragma once

#include "ui/widgets/input_field/input_multiple_scalars.hpp"

namespace xuzy::UI::Widgets::InputField
{
	/**
	* @brief Input widget of multiple type double
	*/
	template <size_t _Size>
	class InputMultipleDoubles : public InputMultipleScalars<double, _Size>
	{
	public:
		/**
		* @brief Constructor
		* @param p_default_value
		* @param p_step
		* @param p_fast_step
		* @param p_label
		* @param p_format
		* @param p_select_all_on_click
		*/
		InputMultipleDoubles
		(
			double p_default_value = 0.0,
			double p_step = 0.1,
			double p_fast_step = 0.0,
			const std::string& p_label = "",
			const std::string& p_format = "%.6f",
			bool p_select_all_on_click = true
		) : InputMultipleScalars<double, _Size>(ImGuiDataType_::ImGuiDataType_Double, p_default_value, p_step, p_fast_step, p_label, p_format, p_select_all_on_click) {}
	};
}