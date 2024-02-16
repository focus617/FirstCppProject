
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Internal
{

ImVec4 Converter::ToImVec4(const Types::Color & p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

Types::Color Converter::ToColor(const ImVec4 & p_value)
{
	return Types::Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 Converter::ToImVec2(const xuzy::Maths::FVector2 & p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

xuzy::Maths::FVector2 Converter::ToFVector2(const ImVec2 & p_value)
{
	return xuzy::Maths::FVector2(p_value.x, p_value.y);
}

} // namespace xuzy::UI::Internal
