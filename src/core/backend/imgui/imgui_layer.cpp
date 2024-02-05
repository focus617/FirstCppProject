#include "pch.h"

#include "imgui_layer.hpp"

#include "app/window_app.hpp"
#include "glfw/window_impl.hpp"

namespace xuzy {

// Our state
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::on_attach() {
  // Setup Dear ImGui
  imgui_init();
  // Load Fonts
  imgui_load_fonts();
}

void ImGuiLayer::on_detach() {
  // Cleanup imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::on_update(){
  begin();
  on_imgui_render();
  end();
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end() {
  ImGuiIO& io = ImGui::GetIO();
  WindowApp& app = (WindowApp&)(App::get());
  io.DisplaySize =
      ImVec2(app.get_window().get_width(), app.get_window().get_height());

  // Rendering
  ImGui::Render();
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::on_imgui_render() {
  // 1. Show the big demo window
  static bool show_demo_window = true;
  ImGui::ShowDemoWindow(&show_demo_window);

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair
  // to create a named window.
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!"
                                    // and append into it.

    ImGui::Text("This is some useful text.");  // Display some text (you can use
                                               // a format strings too)
    ImGui::Checkbox(
        "Demo Window",
        &show_demo_window);  // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f,
                       1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3(
        "clear color",
        (float*)&clear_color);  // Edit 3 floats representing a color

    if (ImGui::Button("Button"))  // Buttons return true when clicked (most
                                  // widgets return true when edited/activated)
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
  }

  // 3. Show another simple window.
  if (show_another_window) {
    ImGui::Begin(
        "Another Window",
        &show_another_window);  // Pass a pointer to our bool variable (the
                                // window will have a closing button that will
                                // clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me")) show_another_window = false;
    ImGui::End();
  }
}

void ImGuiLayer::imgui_init() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
                                           // io.ConfigFlags |=
  //     ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // EnableDocking
  // EnableMulti-Viewport / Platform Windows
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  // When viewports are enabled we tweak WindowRounding/WindowBg
  // so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  WindowApp& app = (WindowApp&)(App::get());
  WindowImpl& win = (WindowImpl&)(app.get_window());
  GLFWwindow* window = static_cast<GLFWwindow*>(win.get_native_window());
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  // Decide GL+GLSL versions
  // GL 3.0 + GLSL 130
  // const char* glsl_version = "#version 130";
  // GL 4.5 + GLSL 130
  const char* glsl_version = "#version 450";
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiLayer::imgui_load_fonts() {
  ImGuiIO& io = ImGui::GetIO();

  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at
  // runtime from the "fonts/" folder. See Makefile.emscripten for details.
  io.Fonts->AddFontDefault();
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("../fonts/DroidSans.ttf", 16.0f);
  // XUZY_CHECK_(nullptr != font) << "Could not load font!";
  // ImGui::PushFont(font);

  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // nullptr, io.Fonts->GetGlyphRangesJapanese());
  // IM_ASSERT(font != nullptr);
}

// void ImGuiLayer::on_event(Ref<Event> event, bool& handled) {
//   handled = false;

//   // Handle global event, e.g. WindowCloseEvent
//   switch (event->get_event_id()) {
//     case EventId::WindowResize:
//       handled = on_window_resize_event(
//           std::static_pointer_cast<WindowResizeEvent>(event));
//       break;

//     case EventId::KeyPressed:
//       handled = on_key_pressed_event(
//           std::static_pointer_cast<KeyPressedEvent>(event));
//       break;

//     case EventId::KeyTyped:
//       handled =
//           on_key_typed_event(std::static_pointer_cast<KeyTypedEvent>(event));
//       break;

//     case EventId::KeyReleased:
//       handled = on_key_released_event(
//           std::static_pointer_cast<KeyReleasedEvent>(event));
//       break;

//     case EventId::MouseButtonPressed:
//       handled = on_mouse_button_pressed_event(
//           std::static_pointer_cast<MouseButtonPressedEvent>(event));
//       break;

//     case EventId::MouseButtonReleased:
//       handled = on_mouse_button_released_event(
//           std::static_pointer_cast<MouseButtonReleasedEvent>(event));
//       break;

//     case EventId::MouseMoved:
//       handled = on_mouse_moved_event(
//           std::static_pointer_cast<MouseMovedEvent>(event));
//       break;

//     case EventId::MouseScrolled:
//       handled = on_mouse_scrolled_event(
//           std::static_pointer_cast<MouseScrolledEvent>(event));
//       break;

//     default:
//       LOG(INFO) << "Other Event: " << *event << std::endl;
//       break;
//   }
// }

// bool ImGuiLayer::on_window_resize_event(Ref<WindowResizeEvent> e) {
//   LOG(INFO) << "Window Resize Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.DisplaySize = ImVec2(e->get_width(), e->get_height());
//   io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
//   glViewport(0, 0, e->get_width(), e->get_height());

//   return true;
// }

// bool ImGuiLayer::on_key_pressed_event(Ref<KeyPressedEvent> e) {
//   LOG(INFO) << "Key Pressed Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.KeysDown[e->get_key_code()] = true;

//   io.KeyCtrl =
//       io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
//       io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
//   io.KeyShift =
//       io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
//   io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] ||
//   io.KeysDown[GLFW_KEY_RIGHT_ALT]; io.KeySuper =
//       io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

//   return true;
// }

// bool ImGuiLayer::on_key_released_event(Ref<KeyReleasedEvent> e) {
//   LOG(INFO) << "Key Released Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.KeysDown[e->get_key_code()] = false;

//   return true;
// }

// bool ImGuiLayer::on_key_typed_event(Ref<KeyTypedEvent> e) {
//   LOG(INFO) << "Key Typed Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   int keycode = e->get_key_code();
//   if (keycode > 0 && keycode < 0x10000)
//     io.AddInputCharacter((unsigned short)keycode);

//   return true;
// }

// bool ImGuiLayer::on_mouse_button_pressed_event(Ref<MouseButtonPressedEvent>
// e) {
//   LOG(INFO) << "Mouse Button Pressed Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.MouseDown[e->get_mouse_button()] = true;

//   return true;
// }

// bool ImGuiLayer::on_mouse_button_released_event(
//     Ref<MouseButtonReleasedEvent> e) {
//   LOG(INFO) << "Mouse Button Released Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.MouseDown[e->get_mouse_button()] = false;

//   return true;
// }

// bool ImGuiLayer::on_mouse_moved_event(Ref<MouseMovedEvent> e) {
//   LOG(INFO) << "Mouse Moved Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.MousePos = ImVec2(e->get_x(), e->get_y());

//   return true;
// }

// bool ImGuiLayer::on_mouse_scrolled_event(Ref<MouseScrolledEvent> e) {
//   LOG(INFO) << "Mouse Scrolled Event: " << *e << std::endl;

//   ImGuiIO& io = ImGui::GetIO();
//   io.MouseWheelH += e->get_x_offset();
//   io.MouseWheel += e->get_y_offset();

//   return true;
// }

}  // namespace xuzy