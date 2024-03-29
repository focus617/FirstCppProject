#pragma once

#include "core/base.hpp"
#include "window/core/window.hpp"

#include "window/backend/glfw/window_impl.hpp"

namespace xuzy::Window {

/**
 * @brief The Context handle the engine features setup
 */
class AppContext {
 public:
  /**
   * Constructor
   * @param p_projectPath
   * @param p_projectName
   */
  AppContext(const std::string& p_app_name, const std::string& p_version);

  /**
   * Destructor
   */
  ~AppContext();

  /**
   * @brief Return built-in GLFWwindow
   */
  GLFWwindow* get_native_window() const;

 public:
  const std::string m_app_name_;
  const std::string m_version_;

  // Decide GL+GLSL versions
  std::string glsl_version = "#version 460";  // GL 4.6

  /* Settings */ 
  Window::WindowProps m_props_;

  Ref<Window::AWindow> m_window_;
};

}  // namespace xuzy::Window