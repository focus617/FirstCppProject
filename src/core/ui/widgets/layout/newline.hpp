#pragma once

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that adds an empty line to the panel
	*/
	class NewLine : public AWidget
	{
	protected:
		void _on_draw_impl() override;
	};
}