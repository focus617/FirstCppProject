#include "ui/widgets/layout/dummy.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Widgets::Layout
{

Dummy::Dummy(const Maths::FVector2& p_size) : size(p_size)
{
}

void Dummy::_on_draw_impl()
{
	ImGui::Dummy(Internal::Converter::ToImVec2(size));
}

} // namespace xuzy::UI::Widgets::Layout
