#include "pch.h"

#include "ui_manager.hpp"

#include "ui/backend/opengl/imgui_include.h"
#include "ui/implot/implot.h"
#include "ui/style/set_style.h"
#include "window/core/window_app.hpp"

namespace xuzy::UI {

UIManager::UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion,
                     UI::Style p_style)
    : ALayer("UIManager"),
      m_glfw_window_{p_glfwWindow},
      m_glsl_version_{p_glslVersion},
      m_style_{p_style} {}

UIManager::~UIManager() {}

void UIManager::on_attach() {
  LOG(INFO) << "UIManager OnAttach";

  // Initialize Dear ImGui
  imgui_init();

  // Setup Dear ImGui style
  apply_style(UI::Style::IMGUI_DARK_STYLE);

  // Setup Docking
  setup_docking();

  // Load Fonts
  load_fonts();
}

void UIManager::on_detach() {
  LOG(INFO) << "UIManager OnDetach";

  // Cleanup imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
}

void UIManager::on_update(Renderer::Times::Timestep p_ts) {}

void UIManager::on_draw() {
  XUZY_PROFILE_FUNCTION();

  if (nullptr != m_canvas_) m_canvas_->on_draw();
}

void UIManager::begin_render() {
  XUZY_PROFILE_FUNCTION();

  // Start a new frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Switch font
  use_font("SimKai");
  // 调整窗口内边距以适应中文文字
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

  // Most "big" widgets share a common width settings by default. See
  // 'Demo->Layout->Widgets Width' for details.
  // e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
  // ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
  // e.g. Leave a fixed amount of width for labels (by passing a negative
  // value), the rest goes to widgets.
  ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
}

void UIManager::end_render() {
  XUZY_PROFILE_FUNCTION();

  ImGui::PopStyleVar(1);

  // Setup display size
  ImGuiIO& io = ImGui::GetIO();
  WindowApp& app = (WindowApp&)(App::get());
  io.DisplaySize = ImVec2(app.get_window().get_size().first,
                          app.get_window().get_size().second);

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

void UIManager::imgui_toolbar() {
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

void UIManager::imgui_init() {
  XUZY_PROFILE_FUNCTION();

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // Disable moving windows by dragging another thing than the title bar
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  io.ConfigViewportsNoAutoMerge = true;
  // 如果不开启io.ConfigViewportsNoAutoMerge可能引起拖动窗口不小心拖动到隐藏的主窗口,
  // 导致子窗口也隐藏的情况。

  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // EnableMulti-Viewport / Platform Windows
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_glfw_window_, true);
  ImGui_ImplOpenGL3_Init(m_glsl_version_.c_str());
}

bool UIManager::load_fonts() {
  load_font("SimKai", "../assets/fonts/SimKai.ttf", m_font_size_pixels_);
  load_font("DroidSans", "../assets/fonts/DroidSans.ttf", m_font_size_pixels_);
  load_font("ProggyClean", "../assets/fonts/ProggyClean.ttf",
            m_font_size_pixels_);
  load_font("Roboto-Medium", "../assets/fonts/Roboto-Medium.ttf",
            m_font_size_pixels_);
  load_font("Cousine-Regular", "../assets/fonts/Cousine-Regular.ttf",
            m_font_size_pixels_);
  load_font("LiberationSans-Regular",
            "../assets/fonts/LiberationSans-Regular.ttf", m_font_size_pixels_);
  return true;
}

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

bool UIManager::load_font(const std::string& p_id, const std::string& p_path,
                          float p_font_size) {
  if (m_fonts_.find(p_id) == m_fonts_.end()) {
    ImGuiIO& io = ImGui::GetIO();

    ImFont* fontInstance = io.Fonts->AddFontFromFileTTF(
        p_path.c_str(), p_font_size, nullptr,
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    if (fontInstance) {
      m_fonts_[p_id] = fontInstance;
      return true;
    }

    LOG(ERROR) << "Could not load font!";
    return false;
  }

  return false;
}

bool UIManager::unload_font(const std::string& p_id) {
  if (m_fonts_.find(p_id) != m_fonts_.end()) {
    m_fonts_.erase(p_id);
    return true;
  }

  return false;
}

bool UIManager::use_font(const std::string& p_id) {
  auto foundFont = m_fonts_.find(p_id);

  if (foundFont != m_fonts_.end()) {
    ImGui::GetIO().FontDefault = foundFont->second;
    return true;
  }

  return false;
}

void UIManager::use_default_font() { ImGui::GetIO().FontDefault = nullptr; }

// Setup Dear ImGui style
void UIManager::apply_style(UI::Style p_style) {
  switch (p_style) {
    case Style::IMGUI_CLASSIC_STYLE:
      ImGui::StyleColorsClassic();
      break;
    case Style::IMGUI_DARK_STYLE:
      ImGui::StyleColorsDark();
      break;
    case Style::IMGUI_LIGHT_STYLE:
      ImGui::StyleColorsLight();
      break;
    case Style::DUNE_DARK:
      set_dune_dark_theme();
      break;
    case Style::ALTERNATIVE_DARK:
      set_alternative_dark_theme();
      break;
    default:
      break;
  }
  // When viewports are enabled we tweak WindowRounding/WindowBg
  // so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
}

bool UIManager::is_docking_enabled() const { return m_enable_docking; }

void UIManager::enable_docking(bool p_value) {
  m_enable_docking = p_value;
  setup_docking();
}

void UIManager::setup_docking() {
  if (m_enable_docking)
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  else
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;

  if (nullptr != m_canvas_) m_canvas_->make_dockspace(m_enable_docking);
}

void UIManager::enable_layout_save(bool p_value) {
  if (p_value)
    ImGui::GetIO().IniFilename = m_layout_config_filename.c_str();
  else
    ImGui::GetIO().IniFilename = nullptr;
}

bool UIManager::is_layout_save_enabled() const {
  return ImGui::GetIO().IniFilename != nullptr;
}

void UIManager::set_layout_config_filename(const std::string& p_filename) {
  m_layout_config_filename = p_filename;
  if (is_layout_save_enabled())
    ImGui::GetIO().IniFilename = m_layout_config_filename.c_str();
}

void UIManager::set_layout_auto_save_frequency(float p_frequency) {
  ImGui::GetIO().IniSavingRate = p_frequency;
}

float UIManager::get_layout_auto_save_frequency(float p_frequeny) {
  return ImGui::GetIO().IniSavingRate;
}

void UIManager::set_canvas(Ref<UI::Canvas> p_canvas) {
  remove_canvas();
  m_canvas_ = p_canvas;

  if (nullptr != m_canvas_) {
    m_canvas_->make_dockspace(m_enable_docking);
  }
}

void UIManager::remove_canvas() { m_canvas_.reset(); }

void UIManager::on_event(Ref<Events::Event> event, bool& handled) {
  handled = false;

  // Handle global event, e.g. WindowCloseEvent
  switch (event->get_event_id()) {
    case Events::EventId::WindowResize:
      handled = on_window_resize_event(
          std::static_pointer_cast<Events::WindowResizeEvent>(event));
      break;

    case Events::EventId::KeyPressed:
      handled = on_key_pressed_event(
          std::static_pointer_cast<Events::KeyPressedEvent>(event));
      break;

    case Events::EventId::KeyTyped:
      handled = on_key_typed_event(
          std::static_pointer_cast<Events::KeyTypedEvent>(event));
      break;

    case Events::EventId::KeyReleased:
      handled = on_key_released_event(
          std::static_pointer_cast<Events::KeyReleasedEvent>(event));
      break;

    case Events::EventId::MouseButtonPressed:
      handled = on_mouse_button_pressed_event(
          std::static_pointer_cast<Events::MouseButtonPressedEvent>(event));
      break;

    case Events::EventId::MouseButtonReleased:
      handled = on_mouse_button_released_event(
          std::static_pointer_cast<Events::MouseButtonReleasedEvent>(event));
      break;

    case Events::EventId::MouseMoved:
      handled = on_mouse_moved_event(
          std::static_pointer_cast<Events::MouseMovedEvent>(event));
      break;

    case Events::EventId::MouseScrolled:
      handled = on_mouse_scrolled_event(
          std::static_pointer_cast<Events::MouseScrolledEvent>(event));
      break;

    default:
      LOG(INFO) << "Other Event: " << *event << std::endl;
      break;
  }
}

bool UIManager::on_window_resize_event(Ref<Events::WindowResizeEvent> e) {
  LOG(INFO) << "Window Resize Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(e->get_width(), e->get_height());
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  glViewport(0, 0, e->get_width(), e->get_height());

  return true;
}

bool UIManager::on_key_pressed_event(Ref<Events::KeyPressedEvent> e) {
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

bool UIManager::on_key_released_event(Ref<Events::KeyReleasedEvent> e) {
  LOG(INFO) << "Key Released Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e->get_key_code()] = false;

  return true;
}

bool UIManager::on_key_typed_event(Ref<Events::KeyTypedEvent> e) {
  LOG(INFO) << "Key Typed Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  int keycode = e->get_key_code();
  if (keycode > 0 && keycode < 0x10000)
    io.AddInputCharacter((unsigned short)keycode);

  return true;
}

bool UIManager::on_mouse_button_pressed_event(
    Ref<Events::MouseButtonPressedEvent> e) {
  LOG(INFO) << "Mouse Button Pressed Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e->get_mouse_button()] = true;

  return true;
}

bool UIManager::on_mouse_button_released_event(
    Ref<Events::MouseButtonReleasedEvent> e) {
  LOG(INFO) << "Mouse Button Released Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e->get_mouse_button()] = false;

  return true;
}

bool UIManager::on_mouse_moved_event(Ref<Events::MouseMovedEvent> e) {
  LOG(INFO) << "Mouse Moved Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e->get_x(), e->get_y());

  return true;
}

bool UIManager::on_mouse_scrolled_event(Ref<Events::MouseScrolledEvent> e) {
  LOG(INFO) << "Mouse Scrolled Event: " << *e << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheelH += e->get_x_offset();
  io.MouseWheel += e->get_y_offset();

  return true;
}

}  // namespace xuzy::UI