#pragma once

#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Widget to display a disabled text on a panel
	*/
	class TextDisabled : public Text
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		*/
		TextDisabled(const std::string& p_content = "");

	protected:
		virtual void _on_draw_impl() override;
	};
}