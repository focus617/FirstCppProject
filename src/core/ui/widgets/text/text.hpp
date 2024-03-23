#pragma once

#include "ui/widgets/AWidget.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Text
{
	/**
	* @brief Simple widget to display text on a panel
	*/
	class XUZY_API Text : public DataWidget<std::string>
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		Text(const std::string& p_content = "");

	protected:
		virtual void _on_draw_impl() override;

	public:
		std::string content;
	};
}