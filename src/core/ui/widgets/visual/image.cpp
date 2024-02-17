#include "ui/widgets/visual/image.hpp"

#include "ui/internal/converter.hpp"

xuzy::UI::Widgets::Visual::Image::Image(uint32_t p_texture_id,
                                        const Maths::FVector2& p_size)
    : texture_id{p_texture_id}, size(p_size) {}

void xuzy::UI::Widgets::Visual::Image::_on_draw_impl() {
  ImGui::Image(texture_id.raw, Internal::Converter::ToImVec2(size),
               ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
}
