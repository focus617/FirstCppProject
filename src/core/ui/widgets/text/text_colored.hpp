#pragma once

#include "ui/widgets/text/text.hpp"
#include "ui/types/color.h"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Widget to display text on a panel that can be colored
	*/
	class TextColored : public Text
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		* @param p_color
		*/
		TextColored(const std::string& p_content = "", 
		const Types::Color& p_color = Types::Color(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		Types::Color color;

	protected:
		virtual void _on_draw_impl() override;
	};
}