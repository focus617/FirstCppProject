#include "ui/widgets/text/text_wrapped.hpp"

namespace xuzy::UI::Widgets::Text
{

TextWrapped::TextWrapped(const std::string & p_content) :
	Text(p_content)
{
}

void TextWrapped::_on_draw_impl()
{
	ImGui::TextWrapped("%s", content.c_str());
}

} // namespace xuzy::UI::Widgets::Text
