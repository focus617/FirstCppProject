#include "ui/plugins/contextual_menu.hpp"

#include "ui/imgui/imgui.h"

namespace xuzy::UI::Plugins
{

void ContextualMenu::execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		draw_widgets();
		ImGui::EndPopup();
	}
}

void ContextualMenu::close()
{
	ImGui::CloseCurrentPopup();
}

}
