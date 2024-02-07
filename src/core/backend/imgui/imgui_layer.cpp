#include "pch.h"

#include "imgui_layer.hpp"

#include "imgui_impl_opengl3_loader.h"
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "app/window_app.hpp"
#include "glfw/window_impl.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace xuzy {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::on_attach() {
  LOG(INFO) << "ImGuiLayer OnAttach";
  // Setup Dear ImGui
  imgui_init();
  // Load Fonts
  imgui_load_fonts();
}

void ImGuiLayer::on_detach() {
  LOG(INFO) << "ImGuiLayer OnDetach";
  // Cleanup imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::on_update() {
  LOG(INFO) << "ImGuiLayer OnUpdate";
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
  // tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data
  // to your main application, or clear/overwrite your copy of the mouse
  // data.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
  // data to your main application, or clear/overwrite your copy of the
  // keyboard data. Generally you may always pass all inputs to dear imgui,
  // and hide them from your application based on those two flags.
  ImGuiIO& io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
  }
}

void ImGuiLayer::begin_render() {
  LOG(INFO) << "ImGuiLayer Begin Render";
  // Start a new frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end_render() {
  LOG(INFO) << "ImGuiLayer End Render";
  // Setup display size
  ImGuiIO& io = ImGui::GetIO();
  WindowApp& app = (WindowApp&)(App::get());
  io.DisplaySize =
      ImVec2(app.get_window().get_width(), app.get_window().get_height());

  // Rendering
  ImGui::Render();
  glClearColor(m_clear_color_.x * m_clear_color_.w,
               m_clear_color_.y * m_clear_color_.w,
               m_clear_color_.z * m_clear_color_.w,
               m_clear_color_.w);  // 设置清除颜色
  glClear(GL_COLOR_BUFFER_BIT);    // 执行清除
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    // Request by multi-viewports
    // Update and Render additional Platform Windows
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::on_imgui_render() {
  LOG(INFO) << "ImGuiLayer OnRender";
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

    // Switch font
    ImGui::PushFont(m_fonts_["SimKai"]);
    // 调整窗口内边距以适应中文文字
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

    // Create a window called "Hello, world!" and append into it.
    ImGui::Begin("你好，世界!");

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
    // Edit 3 floats representing a color
    ImGui::ColorEdit4("clear color", (float*)&m_clear_color_);

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
    ImGui::PopStyleVar(1);
    ImGui::PopFont();
  }

  // // 3. Show another simple window to occupy all screen
  // ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
  // ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);

  // if (show_another_window) {
  //   // Switch font
  //   ImGui::PushFont(m_fonts_["LiberationSans-Regular"]);
  //   // Pass a pointer to our bool variable (the window will have a closing
  //   // button that will clear the bool when clicked)
  //   ImGui::Begin("Window", &show_another_window, ImGuiWindowFlags_MenuBar);
  //   {
  //     // Menu Bar
  //     if (ImGui::BeginMenuBar()) {
  //       if (ImGui::BeginMenu("File")) {
  //         if (ImGui::MenuItem("Open..", "Ctrl+O")) {
  //           /* Do stuff */
  //         }
  //         if (ImGui::MenuItem("Save", "Ctrl+S")) {
  //           /* Do stuff */
  //         }
  //         if (ImGui::MenuItem("Close", "Ctrl+W")) {
  //           show_another_window = false;
  //         }
  //         ImGui::EndMenu();
  //       }
  //       ImGui::EndMenuBar();
  //     }

  //     ImGui::TextDisabled("(?)");
  //     if (ImGui::BeginItemTooltip()) {
  //       ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
  //       ImGui::TextUnformatted("desc");
  //       ImGui::PopTextWrapPos();
  //       ImGui::EndTooltip();
  //     }
  //   }
  //   ImGui::End();
  //   ImGui::PopFont();
  // }
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
  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // EnableDocking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
  // GL 4.6 + GLSL 460
  const char* glsl_version = "#version 460";
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiLayer::imgui_load_fonts() {
  ImGuiIO& io = ImGui::GetIO();

  // If no fonts are loaded, dear imgui will use the default font.
  // io.Fonts->AddFontDefault();

  // - You can also load multiple fonts and use ImGui::PushFont()/PopFont() to
  // select them.
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

  m_fonts_["SimKai"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/SimKai.ttf", m_font_size_pixels_, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  XUZY_CHECK_(nullptr != m_fonts_["SimKai"]) << "Could not load font!";

  m_fonts_["DroidSans"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/DroidSans.ttf", m_font_size_pixels_, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  XUZY_CHECK_(nullptr != m_fonts_["DroidSans"]) << "Could not load font!";

  m_fonts_["Roboto-Medium"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/Roboto-Medium.ttf", m_font_size_pixels_, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  XUZY_CHECK_(nullptr != m_fonts_["Roboto-Medium"]) << "Could not load font!";

  m_fonts_["Cousine-Regular"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/Cousine-Regular.ttf", m_font_size_pixels_, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  XUZY_CHECK_(nullptr != m_fonts_["Cousine-Regular"]) << "Could not load font!";

  ImFontConfig config;
  config.OversampleH = 7;
  config.OversampleV = 1;
  config.GlyphExtraSpacing.x = 1.0f;
  m_fonts_["ProggyClean"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/ProggyClean.ttf", m_font_size_pixels_, &config);
  XUZY_CHECK_(nullptr != m_fonts_["ProggyClean"]) << "Could not load font!";

  m_fonts_["LiberationSans-Regular"] = io.Fonts->AddFontFromFileTTF(
      "../fonts/LiberationSans-Regular.ttf", m_font_size_pixels_, nullptr,
      io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  XUZY_CHECK_(nullptr != m_fonts_["LiberationSans-Regular"])
      << "Could not load font!";
}

void ImGuiLayer::on_event(Ref<Event> event, bool& handled) {
  handled = false;

  // Handle global event, e.g. WindowCloseEvent
  switch (event->get_event_id()) {
    case EventId::WindowResize:
      handled = on_window_resize_event(
          std::static_pointer_cast<WindowResizeEvent>(event));
      break;

    case EventId::KeyPressed:
      handled = on_key_pressed_event(
          std::static_pointer_cast<KeyPressedEvent>(event));
      break;

    case EventId::KeyTyped:
      handled =
          on_key_typed_event(std::static_pointer_cast<KeyTypedEvent>(event));
      break;

    case EventId::KeyReleased:
      handled = on_key_released_event(
          std::static_pointer_cast<KeyReleasedEvent>(event));
      break;

    case EventId::MouseButtonPressed:
      handled = on_mouse_button_pressed_event(
          std::static_pointer_cast<MouseButtonPressedEvent>(event));
      break;

    case EventId::MouseButtonReleased:
      handled = on_mouse_button_released_event(
          std::static_pointer_cast<MouseButtonReleasedEvent>(event));
      break;

    case EventId::MouseMoved:
      handled = on_mouse_moved_event(
          std::static_pointer_cast<MouseMovedEvent>(event));
      break;

    case EventId::MouseScrolled:
      handled = on_mouse_scrolled_event(
          std::static_pointer_cast<MouseScrolledEvent>(event));
      break;

    default:
      LOG(INFO) << "Other Event: " << *event << std::endl;
      break;
  }
}

bool ImGuiLayer::on_window_resize_event(Ref<WindowResizeEvent> e) {
  LOG(INFO) << "Window Resize Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(e->get_width(), e->get_height());
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  glViewport(0, 0, e->get_width(), e->get_height());

  return true;
}

bool ImGuiLayer::on_key_pressed_event(Ref<KeyPressedEvent> e) {
  LOG(INFO) << "Key Pressed Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e->get_key_code()] = true;

  io.KeyCtrl =
      io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io.KeyShift =
      io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
  io.KeySuper =
      io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

  return true;
}

bool ImGuiLayer::on_key_released_event(Ref<KeyReleasedEvent> e) {
  LOG(INFO) << "Key Released Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e->get_key_code()] = false;

  return true;
}

bool ImGuiLayer::on_key_typed_event(Ref<KeyTypedEvent> e) {
  LOG(INFO) << "Key Typed Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  int keycode = e->get_key_code();
  if (keycode > 0 && keycode < 0x10000)
    io.AddInputCharacter((unsigned short)keycode);

  return true;
}

bool ImGuiLayer::on_mouse_button_pressed_event(Ref<MouseButtonPressedEvent> e) {
  LOG(INFO) << "Mouse Button Pressed Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e->get_mouse_button()] = true;

  return true;
}

bool ImGuiLayer::on_mouse_button_released_event(
    Ref<MouseButtonReleasedEvent> e) {
  LOG(INFO) << "Mouse Button Released Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e->get_mouse_button()] = false;

  return true;
}

bool ImGuiLayer::on_mouse_moved_event(Ref<MouseMovedEvent> e) {
  LOG(INFO) << "Mouse Moved Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e->get_x(), e->get_y());

  return true;
}

bool ImGuiLayer::on_mouse_scrolled_event(Ref<MouseScrolledEvent> e) {
  LOG(INFO) << "Mouse Scrolled Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheelH += e->get_x_offset();
  io.MouseWheel += e->get_y_offset();

  return true;
}

}  // namespace xuzy