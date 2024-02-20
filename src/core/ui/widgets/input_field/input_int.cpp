#include "ui/widgets/input_field/input_int.hpp"

xuzy::UI::Widgets::InputField::InputInt::InputInt(int p_default_value,
                                                  int p_step, int p_fast_step,
                                                  const std::string& p_label,
                                                  const std::string& p_format,
                                                  bool p_select_all_on_click)
    : InputSingleScalar<int>(ImGuiDataType_::ImGuiDataType_S32, p_default_value,
                             p_step, p_fast_step, p_label, p_format,
                             p_select_all_on_click) {}
