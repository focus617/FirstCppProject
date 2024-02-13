#include "ui/widgets/AWidget.hpp"

#include "ui/imgui/imgui.h"

namespace xuzy::UI::Widgets {

uint64_t AWidget::__WIDGET_ID_INCREMENT = 0;

AWidget::AWidget() {
  m_widget_id = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void AWidget::link_to(const AWidget& p_widget) {
  m_widget_id = p_widget.m_widget_id;
}

void AWidget::destroy() { m_destroyed = true; }

bool AWidget::is_destroyed() const { return m_destroyed; }

void AWidget::set_parent(Internal::WidgetContainer* p_parent) {
  m_parent = p_parent;
}

bool AWidget::has_parent() const { return m_parent; }

xuzy::UI::Internal::WidgetContainer* AWidget::get_parent() { return m_parent; }

void AWidget::on_draw() {
  if (enabled) {
    _on_draw_impl();

    if (m_auto_execute_plugins) execute_plugins();

    if (!lineBreak) ImGui::SameLine();
  }
}

}  // namespace xuzy::UI::Widgets
