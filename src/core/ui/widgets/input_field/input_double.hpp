#pragma once

#include "ui/widgets/input_field/input_single_scalar.hpp"

namespace xuzy::UI::Widgets::InputField
{
	/**
	* @brief Input widget of type double
	*/
	class InputDouble : public InputSingleScalar<double>
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
		InputDouble
		(
			double p_defaultValue = 0.0,
			double p_step = 0.1,
			double p_fastStep = 0.0,
			const std::string& p_label = "",
			const std::string& p_format = "%.6f",
			bool p_selectAllOnClick = true
		);
	};
}