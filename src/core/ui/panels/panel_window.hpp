#pragma once

#include <memory>

#include "tools/event/event_dispatcher.hpp"

#include "ui/panels/APanelTransformable.hpp"
#include "ui/settings/panel_window_settings.h"

namespace xuzy::UI::Panels
{
	/**
	* @brief A PanelWindow is a panel that is localized and behave like
	* a window (Movable, resizable...)
	*/
	class PanelWindow : public APanelTransformable,
						public std::enable_shared_from_this<PanelWindow>
	{
	public:
		/**
		* @brief Creates the PanelWindow
		* @param p_name
		* @param p_opened
		* @param p_panelSettings
		*/
		PanelWindow
		(
			const std::string& p_name = "",
			bool p_opened = true,
			const Settings::PanelWindowSettings& p_panelSettings = Settings::PanelWindowSettings{}
		);

		std::shared_ptr<PanelWindow> getptr() { return shared_from_this(); }

		/**
		* @brief Open (show) the panel
		*/
		void open();

		/**
		* @brief close (hide) the panel
		*/
		void close();

		/**
		* @brief focus the panel
		*/
		void focus();

		/**
		* @brief Defines the opened state of the window
		* @param p_value
		*/
		void set_opened(bool p_value);

		/**
		* @brief Returns true if the panel is opened
		*/
		bool is_opened() const;

		/**
		* @brief Returns true if the panel is hovered
		*/
		bool is_hovered() const;

		/**
		* @brief Returns true if the panel is focused
		*/
		bool is_focused() const;

		/**
		* @brief Returns true if the panel is appearing
		*/
		bool is_appearing() const;

        /**
        * @brief Scrolls to the bottom of the window
        */
        void scroll_to_bottom();

        /**
        * @brief Scrolls to the top of the window
        */
        void scroll_to_top();

        /**
        * @brief Returns true if the window is scrolled to the bottom
        */
        bool is_scrolled_to_bottom() const;

        /**
        * @brief Returns true if the window is scrolled to the bottom
        */
        bool is_scrolled_to_top() const;

		/**
        * @brief Invoke DrawableOpenEvent
        */
		void invoke_open_event();

		/**
        * @brief Invoke DrawableCloseEvent
        */
		void invoke_close_event();

		/**
        * @brief Event Handler for MenuItem ClickEvent
        */
		void event_handler(Ref<Events::Event> evt, bool& handled);		

	protected:
		virtual void _on_draw_impl() override;

	public:
		std::string name;

		Maths::FVector2 min_size = { 0.f, 0.f };
		Maths::FVector2 max_size = { 0.f, 0.f };

		bool resizable = true;
		bool closable = false;
		bool movable = true;
		bool scrollable = true;
		bool dockable = true;
		bool hide_background = false;
		bool force_horizontal_scrollbar = false;
		bool force_vertical_scrollbar = false;
		bool allow_horizontal_scrollbar = false;
		bool bring_to_front_on_focus = true;
		bool collapsable = false;
		bool allow_inputs = true;
		bool title_bar = true;

		Events::EventDispatcher<void()> event_dispatcher;
		// OvTools::Eventing::Event<> OpenEvent;
		// OvTools::Eventing::Event<> CloseEvent;

	private:
		bool m_opened;
		bool m_hovered;
		bool m_focused;
        bool m_must_scroll_to_bottom = false;
        bool m_must_scroll_to_top = false;
        bool m_scrolled_to_bottom = false;
        bool m_scrolled_to_top = false;
	};

} // namespace xuzy::UI::Panels