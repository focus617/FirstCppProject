#pragma once

namespace xuzy::Window::Cursor
{
	/**
	* @brief Some cursor modes.
	* They defines if the mouse pointer should be visible, locked or normal
	*/
	enum class CursorMode
	{
		NORMAL		= 0x00034001,
		DISABLED	= 0x00034003,
		HIDDEN		= 0x00034002
	};

} // namespace xuzy::Window::Cursor