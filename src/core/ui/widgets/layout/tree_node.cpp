#include "ui/widgets/layout/tree_node.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui.h"

namespace xuzy::UI::Widgets::Layout {

TreeNode::TreeNode(const std::string & p_name, bool p_arrowClickToOpen) :
	DataWidget(name),
	name(p_name),
	m_arrow_click_to_open(p_arrowClickToOpen)
{
	m_auto_execute_plugins = false;
}

void TreeNode::open()
{
	m_should_open = true;
	m_should_close = false;
}

void TreeNode::close()
{
	m_should_close = true;
	m_should_open = false;
}

bool TreeNode::is_opened() const
{
	return m_opened;
}

void TreeNode::_on_draw_impl()
{
	bool prevOpened = m_opened;

	if (m_should_open)
	{
		ImGui::SetNextItemOpen(true);
		m_should_open = false;
	}
	else if (m_should_close)
	{
		ImGui::SetNextItemOpen(false);
		m_should_close = false;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	if (m_arrow_click_to_open)	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (selected)				flags |= ImGuiTreeNodeFlags_Selected;
	if (leaf)					flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx((name + m_widget_id).c_str(), flags);

    if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
    {
		// Invoke ClickedEvent
      	auto click_event =
        	CreateRef<DrawableClickedEvent>(DrawableClickedEvent(getptr()));
    	event_dispatcher.dispatch(click_event);

        if (ImGui::IsMouseDoubleClicked(0))
        {
			// Invoke DoubleClickedEvent
      		auto double_click_event =
        		CreateRef<DrawableDoubleClickedEvent>(DrawableDoubleClickedEvent(getptr()));
    		event_dispatcher.dispatch(double_click_event);
        }
    }

	if (opened)
	{
		if (!prevOpened) {
	    	// Invoke OpenEvent
      		auto open_event =
        		CreateRef<DrawableOpenedEvent>(DrawableOpenedEvent(getptr()));
    		event_dispatcher.dispatch(open_event);
		}
		m_opened = true;

		execute_plugins(); // Manually execute plugins to make plugins considering the TreeNode and no childs

		draw_widgets();

		ImGui::TreePop();
	}
	else
	{
		if (prevOpened) {
	    	// Invoke CloseEvent
      		auto close_event =
        		CreateRef<DrawableClosedEvent>(DrawableClosedEvent(getptr()));
    		event_dispatcher.dispatch(close_event);
		}
		m_opened = false;

		execute_plugins(); // Manually execute plugins to make plugins considering the TreeNode and no childs
	}
}

} // namespace xuzy::UI::Widgets::Layout
