#pragma once

#include <string>

#include "tools/event/event_dispatcher.hpp"

#include "ui/imgui/imgui.h"

#include "ui/plugins/IPlugin.hpp"

namespace xuzy::UI::Plugins
{
	/**
	* @brief Represents a drag and drop source
	*/
	template<typename T>
	class DDSource : public IPlugin
	{
	public:
		/**
		* @brief Create the drag and drop source
		* @param p_identifier
		* @param p_tooltip
		* @param p_data
		*/
		DDSource
		(
			const std::string& p_identifier,
			const std::string& p_tooltip,
			T p_data
		) : identifier(p_identifier), tooltip(p_tooltip), data(p_data)
		{}

		/**
		* @brief Execute the behaviour of the drag and drop source
		*/
		virtual void execute() override
		{
			ImGuiDragDropFlags src_flags = 0;
			// Keep the source displayed as hovered
			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
			// Because our dragging is local, we disable the feature of
			// opening foreign treenodes/tabs while dragging 
			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; 

			if (!has_tool_tip)
			{
			 	// Hide the tooltip
				src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;
			}

			if (ImGui::BeginDragDropSource(src_flags))
			{
				if (!m_is_dragged_)
					DragStartEvent.Invoke();

				m_is_dragged_ = true;

				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
					ImGui::Text(tooltip.c_str());
				ImGui::SetDragDropPayload(identifier.c_str(), &data, sizeof(data));
				ImGui::EndDragDropSource();
			}
			else
			{
				if (m_is_dragged_)
					DragStopEvent.Invoke();

				m_is_dragged_ = false;
			}
		}

		/**
		* @brief Returns true if the drag and drop source is dragged
		*/
		bool Is_dragged() const
		{
			return m_is_dragged_;
		}

	public:
		std::string identifier;
		std::string tooltip;
		T data;

		EventDispatcher<void()> event_dispatcher;
		// OvTools::Eventing::Event<> DragStartEvent;
		// OvTools::Eventing::Event<> DragStopEvent;

		bool has_tool_tip = true;

	private:
		bool m_is_dragged_;
	};

} // namespace xuzy::UI::Plugins