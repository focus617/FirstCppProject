#include "ui/widgets/layout/spacing.hpp"

#include "ui/imgui/imgui.h"

xuzy::UI::Widgets::Layout::Spacing::Spacing(uint16_t p_spaces)
    : spaces(p_spaces) {}

void xuzy::UI::Widgets::Layout::Spacing::_on_draw_impl() {
  for (uint16_t i = 0; i < spaces; ++i) {
    ImGui::Spacing();

    if (i + 1 < spaces) ImGui::SameLine();
  }
}
