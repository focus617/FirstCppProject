#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_impl_opengl3_loader.h"
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "window/layer.hpp"

namespace xuzy {

class XUZY_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void on_attach() override;
  virtual void on_detach() override;
  virtual void on_update() override;
  virtual void on_event(Ref<Event> event, bool& handled) override {}
 
	void begin();
  void on_imgui_render();
	void end();

 private:
  void imgui_init();
  void imgui_load_fonts();
  void imgui_rendering();

  void draw();

 private:
  float m_Time = 0.0f;
};

}  // namespace xuzy