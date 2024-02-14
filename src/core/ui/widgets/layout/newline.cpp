#include "ui/widgets/layout/newline.hpp"

#include "ui/imgui/imgui.h"

void xuzy::UI::Widgets::Layout::NewLine::_on_draw_impl()
{
	ImGui::NewLine();
}
