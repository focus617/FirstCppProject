#include "ui/widgets/input_field/input_text.hpp"

#include "tools/event/drawable_event.hpp"

xuzy::UI::Widgets::InputField::InputText::InputText(const std::string& p_content, const std::string& p_label) :
	DataWidget<std::string>(content), content(p_content), label(p_label)
{
}

void xuzy::UI::Widgets::InputField::InputText::_on_draw_impl()
{
	std::string previousContent = content;

	content.resize(256, '\0');
	bool enterPressed = ImGui::InputText((label + m_widget_id).c_str(), &content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue | (select_all_on_click ? ImGuiInputTextFlags_AutoSelectAll : 0));
	content = content.c_str();

	if (content != previousContent)
	{
		// ContentChangedEvent.Invoke(content);
		auto event = CreateRef<Events::DrawableValueChangedEvent<std::string>>(
        	Events::DrawableValueChangedEvent<std::string>(getptr(), content));
      	event_dispatcher.dispatch(event);

      	// Notify Plugins
      	this->notify_change();
	}

	if (enterPressed)
	{
		// EnterPressedEvent.Invoke(content);
		auto event = CreateRef<Events::DrawableEnterPressedEvent<std::string>>(
        	Events::DrawableEnterPressedEvent<std::string>(getptr(), content));
    	event_dispatcher.dispatch(event);
	}
}