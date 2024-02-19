#pragma once

#include <map>

#include "tools/event/event_dispatcher.hpp"

#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Selection
{
	/**
	* @brief Widget that can display a list of values that the user can select
	*/
	class ComboBox : public DataWidget<int>,
                    public std::enable_shared_from_this<ComboBox>
	{
	public:
		/**
		* @brief Constructor
		* @param p_current_choice
		*/
		ComboBox(int p_current_choice = 0);

		Ref<ComboBox> getptr() { return shared_from_this(); }

	protected:
		void _on_draw_impl() override;

	public:
		std::map<int, std::string> choices;
		int current_choice;

	public:
		Events::EventDispatcher<void()> event_dispatcher;
	};
}