#pragma once

#include "ui/widgets/button/button_image.hpp"
#include "ui/panels/panel_window.hpp"

namespace xuzy::Editor::Panels
{
	class Toolbar : public UI::Panels::PanelWindow
	{
	public:
		/**
		* @brief Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Toolbar
		(
			const std::string& p_title,
			bool p_opened,
			const UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* @brief Custom implementation of the draw method
		*/
		void _on_draw_impl() override;

	private:
		Ref<UI::Widgets::Button::ButtonImage> m_playButton;
		Ref<UI::Widgets::Button::ButtonImage> m_pauseButton;
		Ref<UI::Widgets::Button::ButtonImage> m_stopButton;
		Ref<UI::Widgets::Button::ButtonImage> m_nextButton;
	};

} // namespace xuzy::Editor::Panels