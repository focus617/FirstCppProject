#include "ui/widgets/visual/progressbar.hpp"

#include "ui/internal/converter.hpp"

xuzy::UI::Widgets::Visual::ProgressBar::ProgressBar(
    float p_fraction, const Maths::FVector2& p_size,
    const std::string& p_overlay)
    : fraction(p_fraction), size(p_size), overlay(p_overlay) {}

void xuzy::UI::Widgets::Visual::ProgressBar::_on_draw_impl() {
  ImGui::ProgressBar(fraction, Internal::Converter::ToImVec2(size),
                     !overlay.empty() ? overlay.c_str() : nullptr);
}
