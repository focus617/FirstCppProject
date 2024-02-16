#pragma once

#include <string>

#include "ui/widgets/button/AButton.hpp"
#include "ui/types/color.h"

namespace xuzy::UI::Widgets::Button
{
	/**
	* @brief Small button widget
	*/
	class ButtonSmall : public AButton,
                   		public std::enable_shared_from_this<ButtonSmall>
	{
	public:
		/**
		* @brief Constructor
		* @param p_label
		*/
		ButtonSmall(const std::string& p_label = "");

		std::shared_ptr<ButtonSmall> getptr() { return shared_from_this(); }

	protected:
		void _on_draw_impl() override;

	public:
		std::string label;

		Types::Color idleBackgroundColor;
		Types::Color hoveredBackgroundColor;
		Types::Color clickedBackgroundColor;

		Types::Color textColor;
	};

} // namespace xuzy::UI::Widgets::Button