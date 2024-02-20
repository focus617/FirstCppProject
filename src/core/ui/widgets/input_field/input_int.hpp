#pragma once

#include "ui/widgets/input_field/input_single_scalar.hpp"

namespace xuzy::UI::Widgets::InputField
{
	/**
	* @brief Input widget of type int
	*/
	class InputInt : public InputSingleScalar<int>
	{
	public:
		/**
		* @brief Constructor
		* @param p_defaultValue
		* @param p_step
		* @param p_fastStep
		* @param p_label
		* @param p_format
		* @param p_select_all_on_click
		*/
		InputInt
		(
			int p_defaultValue = 0,
			int p_step = 1,
			int p_fastStep = 0,
			const std::string& p_label = "",
			const std::string& p_format = "%d",
			bool p_select_all_on_click = true
		);
	};
}