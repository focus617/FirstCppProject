#include "ui/widgets/button/button_colored.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/internal/converter.hpp"

xuzy::UI::Widgets::Button::ButtonColored::ButtonColored(
    const std::string& p_label, const Types::Color& p_color,
    const Maths::FVector2& p_size, bool p_enableAlpha)
    : label(p_label),
      color(p_color),
      size(p_size),
      enableAlpha(p_enableAlpha) {}

void xuzy::UI::Widgets::Button::ButtonColored::_on_draw_impl() {
  ImVec4 imColor = Internal::Converter::ToImVec4(color);

  if (ImGui::ColorButton((label + m_widget_id).c_str(), imColor,
                         !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0,
                         Internal::Converter::ToImVec2(size))) {
    // Invoke ClickedEvent
    auto event =
        CreateRef<DrawableClickedEvent>(DrawableClickedEvent(getptr()));
    event_dispatcher.dispatch(event);
  }

  color = Internal::Converter::ToColor(imColor);
}
