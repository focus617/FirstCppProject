#pragma once

namespace xuzy::UI::Settings
{
	/**
	* @brief Data structure to send to the panel window constructor
	* to define its settings
	*/
	struct PanelWindowSettings
	{
		bool closable					= true;
		bool resizable					= true;
		bool movable					= true;
		bool dockable					= true;
		bool scrollable					= true;
		bool hide_background			= false;
		bool force_horizontal_scrollbar	= false;
		bool force_vertical_scrollbar	= false;
		bool allow_horizontal_scrollbar	= false;
		bool bring_to_front_on_focus	= true;
		bool collapsable				= false;
		bool allow_inputs				= true;
		bool title_bar					= true;
		bool auto_size					= false;
	};
	
}