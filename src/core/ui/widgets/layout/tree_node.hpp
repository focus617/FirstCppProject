#pragma once

#include <vector>

#include "tools/event/event_dispatcher.hpp"

#include "ui/internal/widget_container.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that allow columnification
	*/
	class TreeNode : public DataWidget<std::string>, 
					 public Internal::WidgetContainer,
					 public std::enable_shared_from_this<TreeNode>
	{
	public:
		/**
		* @brief Constructor
		* @param p_name
		* @param p_arrowClickToOpen
		*/
		TreeNode(const std::string& p_name = "", bool p_arrowClickToOpen = false);

		std::shared_ptr<TreeNode> getptr() { return shared_from_this(); }

		/**
		* @brief Open the tree node
		*/
		void open();

		/**
		* @brief Close the tree node
		*/
		void close();

		/**
		* @brief Returns true if the TreeNode is currently opened
		*/
		bool is_opened() const;

	protected:
		virtual void _on_draw_impl() override;

	public:
		std::string name;
		bool selected = false;
		bool leaf = false;
		Events::EventDispatcher<void()> event_dispatcher;

	private:
		bool m_arrow_click_to_open = false;
		bool m_should_open = false;
		bool m_should_close = false;
		bool m_opened = false;
	};
}