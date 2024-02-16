#pragma once

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Visual
{
	/**
	* @brief Simple widget that display a separator
	*/
	class Separator : public AWidget
	{
	protected:
		void _on_draw_impl() override;
	};
}