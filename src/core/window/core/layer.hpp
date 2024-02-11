#pragma once

#include "tools/event/event.hpp"

namespace xuzy::Window {

class XUZY_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void on_attach() = 0;
  virtual void on_detach() = 0;
  virtual void on_update() = 0;
  virtual void on_imgui_render() = 0;
  virtual void on_event(Ref<Event> event, bool& handled) = 0;

  inline const std::string& get_name() const { return m_debug_name_; }

 protected:
  std::string m_debug_name_;
};

}  // namespace xuzy::Window