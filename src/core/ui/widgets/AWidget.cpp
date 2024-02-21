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
    if (m_enable_tool_tip && ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 50.0f);
        ImGui::TextUnformatted(m_tool_tip.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    if (m_auto_execute_plugins) execute_plugins();

    if (!lineBreak) ImGui::SameLine();
  }
}

  void AWidget::set_tool_tip_desc(const std::string& p_desc){
    m_tool_tip = p_desc;
  }

  void AWidget::enable_tool_tip(bool p_enable){
    m_enable_tool_tip = p_enable;
  }

  bool AWidget::has_enable_tool_tip() const{
    return m_enable_tool_tip;
  }

}  // namespace xuzy::UI::Widgets
