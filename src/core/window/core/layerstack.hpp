#pragma once

#include <vector>

#include "layer.hpp"
#include "window/API/IDrawable.h"

namespace xuzy::Window {

class XUZY_API LayerStack : public API::IDrawable {
 public:
  LayerStack() = default;
  ~LayerStack();

  /**
   * @brief Draw the canvas and its panels
   */
  virtual void on_draw() override;

  void push_layer(const Ref<ALayer>& layer);
  void pop_layer(const Ref<ALayer>& layer);

  // Overlayer is always on the back, which will process event at first
  void push_overlay(const Ref<ALayer>& layer);
  void pop_overlay(const Ref<ALayer>& layer);

  std::vector<Ref<ALayer>>::iterator begin() { return m_layers_.begin(); }
  std::vector<Ref<ALayer>>::iterator end() { return m_layers_.end(); }
  std::vector<Ref<ALayer>>::reverse_iterator rbegin() {
    return m_layers_.rbegin();
  }
  std::vector<Ref<ALayer>>::reverse_iterator rend() { return m_layers_.rend(); }

 private:
  std::vector<Ref<ALayer>> m_layers_;
  unsigned int m_Layer_insert_index_ = 0;
};

}  // namespace xuzy::Window