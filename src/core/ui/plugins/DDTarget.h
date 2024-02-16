#pragma once

#include <string>

#include "tools/event/event_dispatcher.hpp"

#include "ui/imgui/imgui.h"

#include "ui/plugins/IPlugin.hpp"

namespace xuzy::UI::Plugins
{
	/**
	* @brief Represents a drag and drop target
	*/
	template<typename T>
	class DDTarget : public IPlugin
	{
	public:
		/**
		* @brief Create the drag and drop target
		* @param p_identifier
		*/
		DDTarget(const std::string& p_identifier) : identifier(p_identifier)
		{}

		/**
		* @brief Execute the drag and drop target behaviour
		* @param p_identifier
		*/
		virtual void execute() override
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (!m_is_hovered_)
					HoverStartEvent.Invoke();

				m_is_hovered_ = true;

				ImGuiDragDropFlags target_flags = 0;
				// target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
				
				if (!showYellowRect)
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(identifier.c_str(), target_flags))
				{
					T data = *(T*)payload->Data;
					DataReceivedEvent.Invoke(data);
				}
				ImGui::EndDragDropTarget();
			}
			else
			{
				if (m_is_hovered_)
					HoverEndEvent.Invoke();

				m_is_hovered_ = false;
			}
		}

		/**
		* @brief Returns true if the drag and drop target is hovered 
		* by a drag and drop source
		*/
		bool Is_hovered() const
		{
			return m_is_hovered_;
		}

	public:
		std::string identifier;

		EventDispatcher<void()> event_dispatcher;
		// OvTools::Eventing::Event<T> DataReceivedEvent;
		// OvTools::Eventing::Event<> HoverStartEvent;
		// OvTools::Eventing::Event<> HoverEndEvent;

		bool showYellowRect = true;

	private:
		bool m_is_hovered_;
	};

} // namespace xuzy::UI::Plugins