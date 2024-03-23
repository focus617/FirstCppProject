#pragma once

#include "tools/maths/FVector2.hpp"

#include "ui/widgets/AWidget.hpp"
#include "ui/internal/texture_id.h"

namespace xuzy::UI::Widgets::Visual
{
	/**
	* @brief Simple widget that display an image
	*/
	class XUZY_API Image : public AWidget
	{
	public:
		/**
		* @brief Constructor
		* @param p_textureID
		* @param p_size
		*/
		Image(uint32_t p_texture_id, const Maths::FVector2& p_size);

	protected:
		void _on_draw_impl() override;

	public:
		Internal::TextureID texture_id;
		Maths::FVector2 size;
	};
}