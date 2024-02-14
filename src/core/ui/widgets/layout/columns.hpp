#pragma once

#include <array>

#include "ui/internal/widget_container.hpp"

namespace xuzy::UI::Widgets::Layout
{
	/**
	* @brief Widget that allow columnification
	*/
	template <size_t _Size>
	class Columns : public AWidget, public Internal::WidgetContainer
	{
	public:
		/**
		* Constructor
		*/
		Columns()
		{
			widths.fill(-1.f);
		}

	protected:
		virtual void _draw_impl() override
		{
			ImGui::Columns(static_cast<int>(_Size), ("##" + m_widgetID).c_str(), false);

			int counter = 0;

			collect_garbages();

			for (auto it = m_widgets.begin(); it != m_widgets.end();)
			{
				it->first->on_draw();

				++it;

				if (it != m_widgets.end())
				{
					if (widths[counter] != -1.f)
						ImGui::SetColumnWidth(counter, widths[counter]);

					ImGui::NextColumn();
				}

				++counter;

				if (counter == _Size)
					counter = 0;
			}

			ImGui::Columns(1); // Necessary to not break the layout for following widget
		}

	public:
		std::array<float, _Size> widths;
	};
}