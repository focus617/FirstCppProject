#include "ui/widgets/text/text_labelled.hpp"

namespace xuzy::UI::Widgets::Text
{

TextLabelled::TextLabelled(const std::string& p_content, const std::string& p_label) :
	Text(p_content), label(p_label)
{
}

void TextLabelled::_on_draw_impl()
{
	ImGui::LabelText((label + m_widget_id).c_str(), "%s", content.c_str());
}

}
