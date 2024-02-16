#pragma once

#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Simple widget to display a long text with word-wrap on a panel
	*/
	class TextWrapped : public Text
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		*/
		TextWrapped(const std::string& p_content = "");

	protected:
		virtual void _on_draw_impl() override;
	};
}