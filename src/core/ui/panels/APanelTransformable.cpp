#include "ui/panels/APanelTransformable.hpp"

#include "ui/imgui/imgui.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Panels {

APanelTransformable::APanelTransformable(
    const Maths::FVector2& p_defaultPosition,
    const Maths::FVector2& p_defaultSize,
    UI::Settings::HorizontalAlignment p_defaultHorizontalAlignment,
    UI::Settings::VerticalAlignment p_defaultVerticalAlignment,
    bool p_ignoreConfigFile)
    : m_default_position(p_defaultPosition),
      m_default_size(p_defaultSize),
      m_default_horizontal_alignment(p_defaultHorizontalAlignment),
      m_default_vertical_alignment(p_defaultVerticalAlignment),
      m_ignore_configfile(p_ignoreConfigFile) {}

void APanelTransformable::set_position(const Maths::FVector2& p_position) {
  m_position = p_position;
  m_position_changed = true;
}

void APanelTransformable::set_size(const Maths::FVector2& p_size) {
  m_size = p_size;
  m_size_changed = true;
}

void APanelTransformable::set_alignment(
    UI::Settings::HorizontalAlignment p_horizontalAlignment,
    UI::Settings::VerticalAlignment p_verticalAligment) {
  m_horizontal_alignment = p_horizontalAlignment;
  m_vertical_alignment = p_verticalAligment;
  m_alignment_changed = true;
}

const Maths::FVector2& APanelTransformable::get_position() const {
  return m_position;
}

const Maths::FVector2& APanelTransformable::get_size() const {
  return m_size;
}

UI::Settings::HorizontalAlignment
APanelTransformable::get_horizontal_alignment() const {
  return m_horizontal_alignment;
}

UI::Settings::VerticalAlignment
APanelTransformable::get_vertical_alignment() const {
  return m_vertical_alignment;
}

void APanelTransformable::update_position() {
  if (m_default_position.x != -1.f && m_default_position.y != 1.f) {
    Maths::FVector2 offsettedDefaultPos =
        m_default_position + calculate_position_alignment_offset(true);
    ImGui::SetWindowPos(
        UI::Internal::Converter::ToImVec2(offsettedDefaultPos),
        m_ignore_configfile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
  }

  if (m_position_changed || m_alignment_changed) {
    Maths::FVector2 offset = calculate_position_alignment_offset(false);
    Maths::FVector2 offsettedPos(m_position.x + offset.x,
                                       m_position.y + offset.y);
    ImGui::SetWindowPos(UI::Internal::Converter::ToImVec2(offsettedPos),
                        ImGuiCond_Always);
    m_position_changed = false;
    m_alignment_changed = false;
  }
}

void APanelTransformable::update_size() {
  /*
  if (m_defaultSize.x != -1.f && m_defaultSize.y != 1.f)
      ImGui::SetWindowSize(UI::Internal::Converter::ToImVec2(m_defaultSize),
  m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
  */
  if (m_size_changed) {
    ImGui::SetWindowSize(UI::Internal::Converter::ToImVec2(m_size),
                         ImGuiCond_Always);
    m_size_changed = false;
  }
}

void APanelTransformable::copy_imgui_position() {
  m_position = UI::Internal::Converter::ToFVector2(ImGui::GetWindowPos());
}

void APanelTransformable::copy_imgui_size() {
  m_size = UI::Internal::Converter::ToFVector2(ImGui::GetWindowSize());
}

void APanelTransformable::_on_draw_impl() {
  if (!m_first_frame) {
    if (!auto_size) update_size();
    copy_imgui_size();

    update_position();
    copy_imgui_position();
  }

  m_first_frame = false;
}

Maths::FVector2 APanelTransformable::calculate_position_alignment_offset(
    bool p_default) {
  Maths::FVector2 result(0.0f, 0.0f);

  switch (p_default ? m_default_horizontal_alignment : m_horizontal_alignment) {
    case UI::Settings::HorizontalAlignment::CENTER:
      result.x -= m_size.x / 2.0f;
      break;
    case UI::Settings::HorizontalAlignment::RIGHT:
      result.x -= m_size.x;
      break;
    default:
      break;
  }

  switch (p_default ? m_default_vertical_alignment : m_vertical_alignment) {
    case UI::Settings::VerticalAlignment::MIDDLE:
      result.y -= m_size.y / 2.0f;
      break;
    case UI::Settings::VerticalAlignment::BOTTOM:
      result.y -= m_size.y;
      break;
    default:
      break;
  }

  return result;
}

}  // namespace xuzy::UI::Panels
