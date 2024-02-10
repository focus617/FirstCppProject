#include "imgui.h"
#include "imgui_layer.hpp"
#include "window/core/window_app.hpp"

namespace xuzy::Window {

// Helper to wire demo markers located in code to an interactive browser
typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line,
                                        const char* section, void* user_data);
extern ImGuiDemoMarkerCallback GImGuiDemoMarkerCallback;
extern void* GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback GImGuiDemoMarkerCallback = NULL;
void* GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)                                \
  do {                                                            \
    if (GImGuiDemoMarkerCallback != NULL)                         \
      GImGuiDemoMarkerCallback(__FILE__, __LINE__, section,       \
                               GImGuiDemoMarkerCallbackUserData); \
  } while (0)

// Forward Declarations
static void ShowMenuFile();

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// BeginMainMenuBar() = helper to create menu-bar-sized window at the
// top of the main viewport + call BeginMenuBar() into it.
void ImGuiLayer::show_app_main_menubar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ShowMenuFile();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
      }  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {
      }
      if (ImGui::MenuItem("Copy", "CTRL+C")) {
      }
      if (ImGui::MenuItem("Paste", "CTRL+V")) {
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request
// processing shortcuts)
static void ShowMenuFile() {
  IMGUI_DEMO_MARKER("Examples/Menu");
  ImGui::MenuItem("(demo menu)", NULL, false, false);
  if (ImGui::MenuItem("New")) {
  }
  if (ImGui::MenuItem("Open", "Ctrl+O")) {
  }
  if (ImGui::BeginMenu("Open Recent")) {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    if (ImGui::BeginMenu("More..")) {
      ImGui::MenuItem("Hello");
      ImGui::MenuItem("Sailor");
      if (ImGui::BeginMenu("Recurse..")) {
        ShowMenuFile();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  if (ImGui::MenuItem("Save", "Ctrl+S")) {
  }
  if (ImGui::MenuItem("Save As..")) {
  }

  ImGui::Separator();
  IMGUI_DEMO_MARKER("Examples/Menu/Options");
  if (ImGui::BeginMenu("Options")) {
    static bool enabled = true;
    ImGui::MenuItem("Enabled", "", &enabled);
    ImGui::BeginChild("child", ImVec2(0, 160), ImGuiChildFlags_Border);
    for (int i = 0; i < 10; i++) ImGui::Text("Scrolling Text %d", i);
    ImGui::EndChild();
    static float f = 0.5f;
    static int n = 0;
    ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
    ImGui::InputFloat("Input", &f, 0.1f);
    ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
    ImGui::EndMenu();
  }

  IMGUI_DEMO_MARKER("Examples/Menu/Colors");
  if (ImGui::BeginMenu("Colors")) {
    float sz = ImGui::GetTextLineHeight();
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
      const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
      ImVec2 p = ImGui::GetCursorScreenPos();
      ImGui::GetWindowDrawList()->AddRectFilled(
          p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
      ImGui::Dummy(ImVec2(sz, sz));
      ImGui::SameLine();
      ImGui::MenuItem(name);
    }
    ImGui::EndMenu();
  }

  // Here we demonstrate appending again to the "Options" menu (which we already
  // created above) Of course in this demo it is a little bit silly that this
  // function calls BeginMenu("Options") twice. In a real code-base using it
  // would make senses to use this feature from very different code locations.
  if (ImGui::BeginMenu("Options"))  // <-- Append!
  {
    IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
    static bool b = true;
    ImGui::Checkbox("SomeOption", &b);
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Disabled", false))  // Disabled
  {
    IM_ASSERT(0);
  }
  if (ImGui::MenuItem("Checked", NULL, true)) {
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Quit", "Alt+F4")) {
    ((WindowApp&)(App::get())).close();
  }
}

}  // namespace xuzy::Window