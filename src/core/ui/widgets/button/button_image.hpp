#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/button/AButton.hpp"
#include "ui/types/color.h"

#include "ui/internal/texture_id.h"

namespace xuzy::UI::Widgets::Button
{
	/**
	* @brief Button widget with an image
	*/
	class ButtonImage : public AButton,
                   		public std::enable_shared_from_this<ButtonImage>
	{
	public:
		/**
		* @brief Constructor
		* @param p_textureID
		* @param p_size
		*/
		ButtonImage(uint32_t p_textureID, const Maths::FVector2& p_size);

		std::shared_ptr<ButtonImage> getptr() { return shared_from_this(); }

	protected:
		void _on_draw_impl() override;

	public:
		bool disabled = false;

		UI::Types::Color background = { 0, 0, 0, 0 };
		UI::Types::Color tint = { 1, 1, 1, 1 };

		UI::Internal::TextureID texture_id;
		Maths::FVector2 size;
	};
}