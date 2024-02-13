#pragma once

namespace xuzy::Window::API
{
	/**
	* Interface for any drawable class
	*/
	class IDrawable
	{
	public:
		virtual void on_draw() = 0;

	protected:
		virtual ~IDrawable() = default;
	};
	
} // namespace xuzy::Window::API
