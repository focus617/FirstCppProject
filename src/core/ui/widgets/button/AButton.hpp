#pragma once

#include <string>

#include "tools/event/event_dispatcher.hpp"

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Button
{
	/**
	* @brief Base class for any button widget
	*/
	class AButton : public AWidget
	{
	protected:
		void _on_draw_impl() override = 0;

	public:
		EventDispatcher<void()> event_dispatcher; // DrawableClickedEvent;
	};
}