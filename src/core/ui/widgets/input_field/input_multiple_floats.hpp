#pragma once

#include "ui/widgets/input_field/input_multiple_scalars.hpp"

namespace xuzy::UI::Widgets::InputField
{
	/**
	* @brief Input widget of multiple type float
	*/
	template <size_t _Size>
	class InputMultipleFloats : public InputMultipleScalars<float, _Size>
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
		InputMultipleFloats
		(
			float p_default_value = 0.0f,
			float p_step = 0.1f,
			float p_fast_step = 0.0f,
			const std::string& p_label = "",
			const std::string& p_format = "%.3f",
			bool p_select_all_on_click = true
		) : InputMultipleScalars<float, _Size>(ImGuiDataType_::ImGuiDataType_Float, p_default_value, p_step, p_fast_step, p_label, p_format, p_select_all_on_click) {}
	};
}