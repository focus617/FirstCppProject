#pragma once

#include "tools/event/event_dispatcher.hpp"

#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Selection
{
	/**
	* @brief Checkbox widget that can be checked or not
	*/
	class CheckBox : public DataWidget<bool>,
                     public std::enable_shared_from_this<CheckBox> 
	{
	public:
		/** 
		* @brief Constructor
		* @param p_value
		* @param p_label
		*/
		CheckBox(bool p_value = false, const std::string& p_label = "");

		Ref<CheckBox> getptr() { return shared_from_this(); }

	protected:
		void _on_draw_impl() override;

	public:
		bool value;
		std::string label;

		Events::EventDispatcher<void()> event_dispatcher;
	};

}