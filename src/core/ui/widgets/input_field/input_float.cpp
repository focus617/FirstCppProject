#include "ui/widgets/input_field/input_float.hpp"

xuzy::UI::Widgets::InputField::InputFloat::InputFloat(float p_defaultValue, float p_step, float p_fastStep, const std::string & p_label, const std::string & p_format, bool p_selectAllOnClick)
	: InputSingleScalar<float>(ImGuiDataType_::ImGuiDataType_Float, p_defaultValue, p_step, p_fastStep, p_label, p_format, p_selectAllOnClick)
{
}
