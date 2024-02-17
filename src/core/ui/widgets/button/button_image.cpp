#include "ui/widgets/button/button_image.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui_internal.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Widgets::Button {

ButtonImage::ButtonImage(uint32_t p_textureID, const Maths::FVector2& p_size)
    : textureID{p_textureID}, size(p_size) {}

void ButtonImage::_on_draw_impl() {
  ImVec4 bg = Internal::Converter::ToImVec4(background);
  ImVec4 tn = Internal::Converter::ToImVec4(tint);

  if (ImGui::ImageButton("", textureID.raw, Internal::Converter::ToImVec2(size),
                         ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), bg, tn)) {
    //  disabled ? ImGuiItemFlags_Disabled : 0)) {
    // Invoke ClickedEvent
    auto event = CreateRef<Events::DrawableClickedEvent>(
        Events::DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);
  }
}

}  // namespace xuzy::UI::Widgets::Button
