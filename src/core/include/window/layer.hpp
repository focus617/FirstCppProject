#pragma once

#include "event/event.hpp"

namespace xuzy {

class XUZY_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update() {}
  virtual void on_event(Ref<Event> evt, bool& handled) {}

  inline const std::string& get_name() const { return m_debug_name_; }

 protected:
  std::string m_debug_name_;
};

}  // namespace xuzy