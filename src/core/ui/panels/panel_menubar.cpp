#include "ui/panels/panel_menubar.hpp"

#include "ui/imgui/imgui.h"

namespace xuzy::UI::Panels {

void PanelMenuBar::_on_draw_impl()
{
	if (!m_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		draw_widgets();
		ImGui::EndMainMenuBar();
	}
}

} // namespace xuzy::UI::Panels