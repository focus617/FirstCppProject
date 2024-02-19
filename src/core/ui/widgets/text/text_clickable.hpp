#pragma once

#include "tools/event/event_dispatcher.hpp"

#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	*  @brief @brief Widget to display text on a panel that is also clickable
	*/
	class TextClickable : public Text,
                 		  public std::enable_shared_from_this<TextClickable>
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		*/
		TextClickable(const std::string& p_content = "");

		Ref<TextClickable> getptr() { return shared_from_this(); }

	protected:
		virtual void _on_draw_impl() override;

	public:
	  	Events::EventDispatcher<void()> event_dispatcher;
	};
}