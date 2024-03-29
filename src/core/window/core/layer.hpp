#pragma once

#include "tools/event/event.hpp"
#include "window/API/IDrawable.h"
#include "window/core/time_step.hpp"

namespace xuzy::Window {

class XUZY_API ALayer : public API::IDrawable {
 public:
  ALayer(const std::string& name = "Layer");
  virtual ~ALayer();

  virtual void on_attach() = 0;
  virtual void on_detach() = 0;
  virtual void on_update(Renderer::Times::Timestep p_ts) = 0;
  virtual void on_event(Ref<Events::Event> event, bool& handled) = 0;

  const std::string& get_name() const { return m_debug_name_; }

 protected:
  std::string m_debug_name_;
};

}  // namespace xuzy::Window