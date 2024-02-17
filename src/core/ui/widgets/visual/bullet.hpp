#pragma once

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Visual
{
	/**
	* @brief Simple widget that display a bullet point
	*/
	class Bullet : public AWidget
	{
	protected:
		virtual void _on_draw_impl() override;
	};
}