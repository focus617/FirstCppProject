#include "ui/widgets/text/text_colored.hpp"

#include "ui/internal/converter.hpp"

namespace xuzy::UI::Widgets::Text {

TextColored::TextColored(const std::string& p_content,
                         const Types::Color& p_color)
    : Text(p_content), color(p_color) {}

void TextColored::_on_draw_impl() {
  ImGui::TextColored(Internal::Converter::ToImVec4(color), "%s",
                     content.c_str());
}

}  // namespace xuzy::UI::Widgets::Text