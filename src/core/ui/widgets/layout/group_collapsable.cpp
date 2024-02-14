#include "ui/widgets/layout/group_collapsable.hpp"
#include "ui/imgui/imgui_internal.h"
#include "tools/event/drawable_event.hpp"

namespace xuzy::UI::Widgets::Layout
{

GroupCollapsable::GroupCollapsable(const std::string & p_name) :
	name(p_name)
{
}

void GroupCollapsable::_on_draw_impl()
{
	bool previouslyOpened = opened;

	if (ImGui::CollapsingHeader(name.c_str(), closable ? &opened : nullptr))
		Group::_on_draw_impl();

	if (opened != previouslyOpened)
	{
		if (opened){
	    	// Invoke OpenEvent
      		auto event =
        		CreateRef<DrawableOpenedEvent>(DrawableOpenedEvent(getptr()));
    		event_dispatcher.dispatch(event);
		}
		else {
	    	// Invoke CloseEvent
      		auto event =
        		CreateRef<DrawableClosedEvent>(DrawableClosedEvent(getptr()));
    		event_dispatcher.dispatch(event);
		}
	}
}

} // namespace xuzy::UI::Widgets::Layout
