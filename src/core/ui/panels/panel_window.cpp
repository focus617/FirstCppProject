#include "ui/panels/panel_window.hpp"

#include "tools/event/drawable_event.hpp"
#include "ui/imgui/imgui_internal.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Panels {

PanelWindow::PanelWindow(
    const std::string& p_name, bool p_opened,
    const Settings::PanelWindowSettings& p_floatingPanelSettings)
    : name(p_name),
      resizable(p_floatingPanelSettings.resizable),
      closable(p_floatingPanelSettings.closable),
      movable(p_floatingPanelSettings.movable),
      scrollable(p_floatingPanelSettings.scrollable),
      dockable(p_floatingPanelSettings.dockable),
      hide_background(p_floatingPanelSettings.hide_background),
      force_horizontal_scrollbar(
          p_floatingPanelSettings.force_horizontal_scrollbar),
      force_vertical_scrollbar(
          p_floatingPanelSettings.force_vertical_scrollbar),
      allow_horizontal_scrollbar(
          p_floatingPanelSettings.allow_horizontal_scrollbar),
      bring_to_front_on_focus(p_floatingPanelSettings.bring_to_front_on_focus),
      collapsable(p_floatingPanelSettings.collapsable),
      allow_inputs(p_floatingPanelSettings.allow_inputs),
      m_opened(p_opened) {
  auto_size = p_floatingPanelSettings.auto_size;
}

void PanelWindow::open() {
  if (!m_opened) {
    m_opened = true;
    invoke_open_event();
  }
}

void PanelWindow::close() {
  if (m_opened) {
    m_opened = false;
    invoke_close_event();
  }
}

void PanelWindow::focus() {
  ImGui::SetWindowFocus((name + m_panel_id).c_str());
}

void PanelWindow::set_opened(bool p_value) {
  if (p_value != m_opened) {
    m_opened = p_value;

    if (m_opened)
      invoke_open_event();
    else
      invoke_close_event();
  }
}

bool PanelWindow::is_opened() const { return m_opened; }

bool PanelWindow::is_hovered() const { return m_hovered; }

bool PanelWindow::is_focused() const { return m_focused; }

bool PanelWindow::is_appearing() const {
  if (auto window = ImGui::FindWindowByName((name + get_panel_id()).c_str());
      window)
    return window->Appearing;
  else
    return false;
}

void PanelWindow::scroll_to_bottom() { m_must_scroll_to_bottom = true; }

void PanelWindow::scroll_to_top() { m_must_scroll_to_top = true; }

bool PanelWindow::is_scrolled_to_bottom() const { return m_scrolled_to_bottom; }

bool PanelWindow::is_scrolled_to_top() const { return m_scrolled_to_top; }

void PanelWindow::invoke_open_event() {
  LOG(INFO) << name << get_panel_id() << " Panel opened." << std::endl;

  // Invoke OpenEvent
  auto open_event = CreateRef<Events::DrawableOpenedEvent>(
      Events::DrawableOpenedEvent(getptr()));
  event_dispatcher.dispatch(open_event);
}

void PanelWindow::invoke_close_event() {
  LOG(INFO) << name << get_panel_id() << " Panel closed." << std::endl;

  // Invoke CloseEvent
  auto close_event = CreateRef<Events::DrawableClosedEvent>(
      Events::DrawableClosedEvent(getptr()));
  event_dispatcher.dispatch(close_event);
}

void PanelWindow::on_event(Ref<Events::Event> evt, bool& handled) {
  if(!is_hovered() || !is_focused()) return;
  
  LOG(INFO) << "Event: " << *evt << std::endl;

  handled = false;

  switch (evt->get_event_id()) {
    case Events::EventId::DrawableClicked:
      set_opened(true);
      handled = true;
      break;
    case Events::EventId::DrawableValueChanged:
      handled = true;
      break;
    default:
      break;
  }
}

void PanelWindow::_on_draw_impl() {
  if (m_opened) {
    int windowFlags = ImGuiWindowFlags_None;

    if (!resizable) windowFlags |= ImGuiWindowFlags_NoResize;
    if (!movable) windowFlags |= ImGuiWindowFlags_NoMove;
    if (!dockable) windowFlags |= ImGuiWindowFlags_NoDocking;
    if (hide_background) windowFlags |= ImGuiWindowFlags_NoBackground;
    if (force_horizontal_scrollbar)
      windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    if (force_vertical_scrollbar)
      windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
    if (allow_horizontal_scrollbar)
      windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
    if (!bring_to_front_on_focus)
      windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (!collapsable) windowFlags |= ImGuiWindowFlags_NoCollapse;
    if (!allow_inputs) windowFlags |= ImGuiWindowFlags_NoInputs;
    if (!scrollable)
      windowFlags |=
          ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
    if (!title_bar) windowFlags |= ImGuiWindowFlags_NoTitleBar;

    ImVec2 minSizeConstraint = Internal::Converter::ToImVec2(min_size);
    ImVec2 maxSizeConstraint = Internal::Converter::ToImVec2(max_size);

    /* Cancel constraint if x or y is <= 0.f */
    if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
      minSizeConstraint = {0.0f, 0.0f};

    if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
      maxSizeConstraint = {10000.f, 10000.f};

    ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

    if (ImGui::Begin((name + m_panel_id).c_str(),
                     closable ? &m_opened : nullptr, windowFlags)) {
      content_region_size = 
        Internal::Converter::ToFVector2(ImGui::GetContentRegionAvail());

      m_hovered = ImGui::IsWindowHovered();
      m_focused = ImGui::IsWindowFocused();

      auto scrollY = ImGui::GetScrollY();

      m_scrolled_to_bottom = scrollY == ImGui::GetScrollMaxY();
      m_scrolled_to_top = scrollY == 0.0f;

      if (!m_opened) invoke_close_event();

      APanelTransformable::_on_draw_impl();

      if (m_must_scroll_to_bottom) {
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
        m_must_scroll_to_bottom = false;
      }

      if (m_must_scroll_to_top) {
        ImGui::SetScrollY(0.0f);
        m_must_scroll_to_top = false;
      }

      draw_widgets();
    }

    ImGui::End();
  }
}

}  // namespace xuzy::UI::Panels