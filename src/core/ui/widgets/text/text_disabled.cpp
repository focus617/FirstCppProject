#include "ui/widgets/text/text_disabled.hpp"

namespace xuzy::UI::Widgets::Text {

TextDisabled::TextDisabled(const std::string& p_content) : Text(p_content) {}

void TextDisabled::_on_draw_impl() {
  ImGui::TextDisabled("%s", content.c_str());
}

}  // namespace xuzy::UI::Widgets::Text
