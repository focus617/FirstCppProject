#include <algorithm>

#include "ui/internal/widget_container.hpp"

namespace xuzy::UI::Internal {

WidgetContainer::~WidgetContainer()
{
	remove_all_widgets();
}

void WidgetContainer::remove_widget(Ref<Widgets::AWidget>& p_widget)
{
	auto found = std::find(m_widgets.begin(), m_widgets.end(), p_widget);

	if (found != m_widgets.end())
	{
		m_widgets.erase(found);
	}
}

void WidgetContainer::remove_all_widgets()
{
	m_widgets.clear();
}

void WidgetContainer::consider_widget(Ref<Widgets::AWidget>& p_widget)
{
	p_widget->set_parent(this);
	m_widgets.emplace_back(p_widget);
}

void WidgetContainer::unconsider_widget(Ref<Widgets::AWidget>& p_widget)
{
	auto found = std::find(m_widgets.begin(), m_widgets.end(), p_widget);

	if (found != m_widgets.end())
	{
		m_widgets.erase(found);
		p_widget->set_parent(nullptr);
	}
}

void WidgetContainer::draw_widgets()
{
    if (m_reversed_draw_order)
    {
        for (auto it = m_widgets.rbegin(); it != m_widgets.rend(); ++it)
            (*it)->on_draw();
    }
    else
    {
        for (const auto& widget : m_widgets)
            widget->on_draw();
    }
}

void WidgetContainer::reverse_draw_order(const bool reversed)
{
    m_reversed_draw_order = reversed;
}

std::vector<Ref<UI::Widgets::AWidget>>& WidgetContainer::GetWidgets()
{
	return m_widgets;
}

} // namespace xuzy::UI::Internal
