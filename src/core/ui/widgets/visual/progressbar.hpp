#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Widgets::Visual
{
	/**
	* @brief Simple widget that display a progress bar
	*/
	class ProgressBar : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_fraction
		* @param p_size
		* @param p_overlay
		*/
		ProgressBar(float p_fraction = 0.0f, 
				const Maths::FVector2& p_size = { 0.0f, 0.0f }, 
				const std::string& p_overlay = "");

	protected:
		void _on_draw_impl() override;

	public:
		float fraction;
		Maths::FVector2 size;
		std::string overlay;
	};
}