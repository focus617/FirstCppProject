#pragma once

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that adds a space to the panel line
	*/
	class Spacing : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_spaces
		*/
		Spacing(uint16_t p_spaces = 1);

	protected:
		void _on_draw_impl() override;

	public:
		uint16_t spaces = 1;
	};
}