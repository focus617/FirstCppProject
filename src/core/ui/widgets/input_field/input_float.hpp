#pragma once

#include "ui/widgets/input_field/input_single_scalar.hpp"

namespace xuzy::UI::Widgets::InputField
{
	/**
	* @brief Input widget of type float
	*/
	class InputFloat : public InputSingleScalar<float>
	{
	public:
		/**
		* @brief Constructor
		* @param p_defaultValue
		* @param p_step
		* @param p_fastStep
		* @param p_label
		* @param p_format
		* @param p_selectAllOnClick
		*/
		InputFloat
		(
			float p_defaultValue = 0.0f,
			float p_step = 0.1f,
			float p_fastStep = 0.0f,
			const std::string& p_label = "",
			const std::string& p_format = "%.3f",
			bool p_selectAllOnClick = true
		);
	};
}