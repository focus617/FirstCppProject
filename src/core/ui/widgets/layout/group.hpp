#pragma once

#include <vector>

#include "ui/internal/widget_container.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that can contains other widgets
	*/
	class Group : public AWidget, public Internal::WidgetContainer
	{
	protected:
		virtual void _on_draw_impl() override;
	};
}