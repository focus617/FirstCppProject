#include "pch.h"

#include "window/glfw_imgui/imgui_layer.hpp"

#include "imgui_include.h"
#include "window/core/window_app.hpp"
#include "window/glfw_imgui/window_impl.hpp"

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
  // Start a new frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Switch font
  ImGui::PushFont(m_fonts_["SimKai"]);
  // 调整窗口内边距以适应中文文字
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

  // Most "big" widgets share a common width settings by default. See
  // 'Demo->Layout->Widgets Width' for details.
  // e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
  // ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
  // e.g. Leave a fixed amount of width for labels (by passing a negative
  // value), the rest goes to widgets.
  ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

  // ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
  // ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
}

void ImGuiLayer::end_render() {
  ImGui::PopStyleVar(1);
  ImGui::PopFont();

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
  // Create an explicit docking node within an existing window.
  // - Drag from window title bar or their tab to dock/undock. Hold SHIFT to
  // disable docking.
  // - Drag from window menu button (upper-left button) to undock an entire node
  // (all windows).
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  if (show_app_main_menu_bar) show_app_main_menubar();

  if (show_app_toolbar) imgui_toolbar();
}

void ImGuiLayer::imgui_toolbar() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  auto& colors = ImGui::GetStyle().Colors;
  const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
  ImGui::PushStyleColor(
      ImGuiCol_ButtonHovered,
      ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
  const auto& buttonActive = colors[ImGuiCol_ButtonActive];
  ImGui::PushStyleColor(
      ImGuiCol_ButtonActive,
      ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

  ImGui::Begin("##toolbar", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse);

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);
  ImGui::End();
}

void ImGuiLayer::imgui_init() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

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

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg
  // so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  set_dark_theme_colors();

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

void ImGuiLayer::set_dark_theme_colors() {
  auto& colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
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