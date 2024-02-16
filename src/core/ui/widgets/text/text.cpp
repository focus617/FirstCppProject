#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{

Text::Text(const std::string & p_content) :
	DataWidget(content),
	content(p_content)
{
}

void Text::_on_draw_impl()
{
	ImGui::Text("%s", content.c_str());
}

}