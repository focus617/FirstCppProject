#pragma once

#include <string>

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/button/AButton.hpp"
#include "ui/types/color.h"

namespace xuzy::UI::Widgets::Button
{
	/**
	* @brief Simple button widget
	*/
	class Button : public AButton,
                   public std::enable_shared_from_this<Button>
	{
	public:
		/**
		* @brief Constructor
		* @param p_label
		* @param p_size
		* @param p_disabled
		*/
		Button(const std::string& p_label = "", 
			   const Maths::FVector2& p_size = Maths::FVector2(0.f, 0.f), 
			   bool p_disabled = false);

		std::shared_ptr<Button> getptr() { return shared_from_this(); }

	protected:
		void _on_draw_impl() override;

	public:
		std::string label;
		Maths::FVector2 size;
		bool disabled = false;

		Types::Color idleBackgroundColor;
		Types::Color hoveredBackgroundColor;
		Types::Color clickedBackgroundColor;

		Types::Color textColor;
	};
}