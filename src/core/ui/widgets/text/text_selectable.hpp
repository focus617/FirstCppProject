#pragma once

#include "tools/event/event_dispatcher.hpp"

#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Simple widget to display a selectable text on a panel
	*/
	class TextSelectable : public Text,
                 		   public std::enable_shared_from_this<TextSelectable>
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		* @param p_selected
		* @param p_disabled
		*/
		TextSelectable(const std::string& p_content = "", 
					   bool p_selected = false, 
					   bool p_disabled = false);

		Ref<TextSelectable> getptr() { return shared_from_this(); }

	protected:
		virtual void _on_draw_impl() override;

	public:
		bool selected;
		bool disabled;

		Events::EventDispatcher<void()> event_dispatcher;
	};
}