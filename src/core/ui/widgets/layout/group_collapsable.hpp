#pragma once

#include <vector>

#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/layout/group.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that can contains other widgets and is collapsable
	*/
	class GroupCollapsable : public Group,
                 public std::enable_shared_from_this<GroupCollapsable>
	{
	public:
		/**
		* @brief Constructor
		* @param p_name
		*/
		GroupCollapsable(const std::string& p_name = "");

		std::shared_ptr<GroupCollapsable> getptr() {
			return shared_from_this(); 
		}

	protected:
		virtual void _on_draw_impl() override;

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		EventDispatcher<void()> event_dispatcher;
	};
}