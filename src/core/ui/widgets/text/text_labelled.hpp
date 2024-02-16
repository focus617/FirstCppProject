#pragma once

#include "ui/widgets/text/text.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Simple widget to display a labelled text on a panel
	*/
	class TextLabelled : public Text
	{
	public:
		/**
		* @brief Constructor
		* @param p_content
		* @param p_label
		*/
		TextLabelled(const std::string& p_content = "", 
					 const std::string& p_label = "");

	protected:
		virtual void _on_draw_impl() override;

	public:
		std::string label;
	};
}