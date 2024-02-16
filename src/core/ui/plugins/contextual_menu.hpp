#pragma once

#include "ui/plugins/IPlugin.hpp"
#include "ui/internal/widget_container.hpp"
#include "ui/widgets/menu/menu_list.hpp"
#include "ui/widgets/menu/menu_item.hpp"

namespace xuzy::UI::Plugins
{
	/**
	* @brief A simple plugin that will show a contextual menu on right click
	* You can add widgets to a contextual menu
	*/
	class ContextualMenu : public IPlugin, public Internal::WidgetContainer
	{
	public:
		/**
		* @brief Execute the plugin
		*/
		void execute();

		/**
		* @brief Force close the contextual menu
		*/
		void close();
	};
}
