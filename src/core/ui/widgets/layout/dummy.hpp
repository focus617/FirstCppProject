#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Dummy widget that takes the given size as space in the panel
	*/
	class Dummy : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_size
		*/
		Dummy(const Maths::FVector2& p_size = { 0.0f, 0.0f });

	protected:
		void _on_draw_impl() override;

	public:
		Maths::FVector2 size;
	};
}