#include "ui/widgets/button/button.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui_internal.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Widgets::Button {

Button::Button(const std::string& p_label, const Maths::FVector2& p_size,
               bool p_disabled)
    : label(p_label), size(p_size), disabled(p_disabled) {
  auto& style = ImGui::GetStyle();

  idleBackgroundColor =
      Internal::Converter::ToColor(style.Colors[ImGuiCol_Button]);
  hoveredBackgroundColor =
      Internal::Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
  clickedBackgroundColor =
      Internal::Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
  textColor = Internal::Converter::ToColor(style.Colors[ImGuiCol_Text]);
}

void Button::_on_draw_impl() {
  auto& style = ImGui::GetStyle();

  auto defaultIdleColor = style.Colors[ImGuiCol_Button];
  auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
  auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
  auto defaultTextColor = style.Colors[ImGuiCol_Text];

  style.Colors[ImGuiCol_Button] =
      UI::Internal::Converter::ToImVec4(idleBackgroundColor);
  style.Colors[ImGuiCol_ButtonHovered] =
      UI::Internal::Converter::ToImVec4(hoveredBackgroundColor);
  style.Colors[ImGuiCol_ButtonActive] =
      UI::Internal::Converter::ToImVec4(clickedBackgroundColor);
  style.Colors[ImGuiCol_Text] = UI::Internal::Converter::ToImVec4(textColor);

  if (ImGui::ButtonEx((label + m_widget_id).c_str(),
                      Internal::Converter::ToImVec2(size),
                      // disabled ? ImGuiButtonFlags_Disabled : 0))
                      disabled ? ImGuiItemFlags_Disabled : 0)) {
    // Invoke ClickedEvent
    auto event =
        CreateRef<DrawableClickedEvent>(DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);
  }

  style.Colors[ImGuiCol_Button] = defaultIdleColor;
  style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
  style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
  style.Colors[ImGuiCol_Text] = defaultTextColor;
}

}  // namespace xuzy::UI::Widgets::Button
