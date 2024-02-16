#include "ui/widgets/text/text_clickable.hpp"

#include "tools/event/drawable_event.hpp"

namespace xuzy::UI::Widgets::Text
{

TextClickable::TextClickable(const std::string & p_content) :
	Text(p_content)
{
}

void TextClickable::_on_draw_impl()
{
	bool useless = false;

    if (ImGui::Selectable((content + m_widget_id).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
    {
        if (ImGui::IsMouseDoubleClicked(0))
        {
            // Invoke DoubleClickedEvent
            auto event =
                CreateRef<DrawableDoubleClickedEvent>(
                    DrawableDoubleClickedEvent(getptr()));
            event_dispatcher.dispatch(event);
        }
        else
        {
            // Invoke ClickedEvent
            auto event =
                CreateRef<DrawableClickedEvent>(DrawableClickedEvent(getptr()));
            event_dispatcher.dispatch(event);
        }
    }
}

} // namespace xuzy::UI::Widgets::Text
