#include "canvas.hpp"

#include "tools/event/key_event.hpp"
#include "window/core/inputs/input_manager.hpp"

namespace xuzy::UI {

void Canvas::on_draw() {
  if (m_is_dockspace_) {
    // Create an explicit docking node within an existing window.
    // - Drag from window title bar or their tab to dock/undock. Hold SHIFT to
    // disable docking.
    // - Drag from window menu button (upper-left button) to undock an entire
    // node (all windows).

    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("##dockspace", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::SetWindowPos({0.f, 0.f});
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetWindowSize({(float)displaySize.x, (float)displaySize.y});
    ImGui::End();

    ImGui::PopStyleVar(3);
  }
}

void Canvas::make_dockspace(bool p_state) { m_is_dockspace_ = p_state; }

bool Canvas::is_dockspace() const { return m_is_dockspace_; }

}  // namespace xuzy::UI