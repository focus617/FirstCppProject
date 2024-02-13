#pragma once

#include "ui/imgui/imgui.h"
#include "window/core/inputs/input_manager.hpp"
#include "window/core/layer.hpp"

namespace xuzy::Window {

class ExampleLayer : public ALayer {
 public:
  ExampleLayer() : Layer("Example") {}

  void on_attach() override { LOG(INFO) << "ExampleLayer OnAttach"; }
  void on_detach() override { LOG(INFO) << "ExampleLayer OnDetach"; }

  void on_update() override {
    if (Inputs::InputManager::is_key_pressed(Inputs::Key::Tab))
      LOG(INFO) << "Tab key is pressed (poll)!";
  }

  void on_event(Ref<Event> event, bool& handled) override {
    if (event->get_event_id() == EventId::KeyPressed) {
      Ref<KeyPressedEvent> e = std::static_pointer_cast<KeyPressedEvent>(event);
      if (e->get_key_code() == Inputs::Key::Tab) {
        LOG(INFO) << "Tab key is pressed (event)!";
        handled = true;
      }
      LOG(INFO) << "KeyCode is " << (char)(e->get_key_code());
    }
  }

  void on_draw() override {
    // LOG(INFO) << "ExampleLayer OnRender";

    // Our state
    static bool show_demo_window = true;
    static bool show_another_window = true;

    // 1. Show the big demo window
    ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a window that using Chinese.
    // We use a Begin/End pair to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;
      const int buffer_size = 1024;
      char* buf = new char[buffer_size];
      strcpy(buf, "默认字符串");

      // Create a window called "Hello, world!" and append into it.
      ImGui::Begin("你好，世界!", &show_another_window,
                   ImGuiWindowFlags_MenuBar);

      // Menu Bar
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          // Disabling fullscreen would allow the window to be moved to the
          // front of other windows, which we can't undo at the moment without
          // finer window depth/z control. ImGui::MenuItem("Fullscreen", NULL,
          // &opt_fullscreen_persistant);
          if (ImGui::MenuItem("New", "Ctrl+N")) {
            /* Do stuff */
          }
          if (ImGui::MenuItem("Open..", "Ctrl+O")) {
            /* Do stuff */
          }
          if (ImGui::MenuItem("Save", "Ctrl+S")) {
            /* Do stuff */
          }
          if (ImGui::MenuItem("Close", "Ctrl+W")) {
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      // Display some text (you can use a format strings too)
      ImGui::Text("这是第一个中文提示。");
      ImGui::Text("输入:");
      ImGui::SameLine();
      ImGui::InputText("OK", buf, buffer_size);
      ImGui::SameLine();
      if (ImGui::Button("确认")) strcpy(buf, "已确认。");

      // Button to control another window
      if (show_another_window) {
        if (ImGui::Button("关闭另一个窗口")) {
          show_another_window = false;
        }
      } else {
        if (ImGui::Button("打开另一个窗口")) {
          show_another_window = true;
        }
      }

      // Edit bools storing our window open/close state
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);

      // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("desc");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }

      // Buttons return true when clicked
      // (most widgets return true when edited/activated)
      if (ImGui::Button("Button")) counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGuiIO& io = ImGui::GetIO();
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);

      ImGui::SeparatorText("CORRECT");
      ImGui::DebugTextEncoding((const char*)u8"中文");

      // Generate samples and plot them
      float samples[100];
      for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
      ImGui::PlotLines("Samples", samples, 100);

      ImGui::End();

      delete[] buf;
    }
  }
};

}  // namespace xuzy::Window