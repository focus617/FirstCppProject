#include <algorithm>

#include "ui/internal/widget_container.hpp"

namespace xuzy::UI::Internal {

WidgetContainer::~WidgetContainer()
{
	remove_all_widgets();
}

void WidgetContainer::remove_widget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<xuzy::UI::Widgets::AWidget*, Internal::MemoryMode>& p_pair)
	{ 
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		if (found->second == Internal::MemoryMode::INTERNAL_MANAGMENT)
			delete found->first;

		m_widgets.erase(found);
	}
}

void WidgetContainer::remove_all_widgets()
{
	std::for_each(m_widgets.begin(), m_widgets.end(), [](auto& pair)
	{
		if (pair.second == Internal::MemoryMode::INTERNAL_MANAGMENT)
			delete pair.first;
	});

	m_widgets.clear();
}

void WidgetContainer::consider_widget(Widgets::AWidget & p_widget, bool p_manageMemory)
{
	m_widgets.emplace_back(std::make_pair(&p_widget, p_manageMemory ? MemoryMode::INTERNAL_MANAGMENT : MemoryMode::EXTERNAL_MANAGMENT));
	p_widget.set_parent(this);
}

void WidgetContainer::unconsider_widget(Widgets::AWidget & p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<xuzy::UI::Widgets::AWidget*, Internal::MemoryMode>& p_pair)
	{
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.set_parent(nullptr);
		m_widgets.erase(found);
	}
}

void WidgetContainer::collect_garbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::pair<xuzy::UI::Widgets::AWidget*, Internal::MemoryMode>& p_item)
	{
		bool to_destroy = p_item.first && p_item.first->is_destroyed();

		if (to_destroy && p_item.second == Internal::MemoryMode::INTERNAL_MANAGMENT)
			delete p_item.first;

		return to_destroy;
	}), m_widgets.end());
}

void WidgetContainer::draw_widgets()
{
	collect_garbages();

    if (m_reversedDrawOrder)
    {
        for (auto it = m_widgets.crbegin(); it != m_widgets.crend(); ++it)
            it->first->on_draw();
    }
    else
    {
        for (const auto& widget : m_widgets)
            widget.first->on_draw();
    }
}

void WidgetContainer::reverse_draw_order(const bool reversed)
{
    m_reversedDrawOrder = reversed;
}

std::vector<std::pair<xuzy::UI::Widgets::AWidget*, MemoryMode>>& WidgetContainer::GetWidgets()
{
	return m_widgets;
}

} // namespace xuzy::UI::Internal
