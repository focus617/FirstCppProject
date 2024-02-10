#pragma once

#include <vector>

#include "layer.hpp"

namespace xuzy::Window {

class XUZY_API LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void push_layer(const Ref<Layer>& layer);
  void pop_layer(const Ref<Layer>& layer);

  // Overlayer is always on the back, which will process event at first
  void push_overlay(const Ref<Layer>& layer);
  void pop_overlay(const Ref<Layer>& layer);

  std::vector<Ref<Layer>>::iterator begin() { return m_layers_.begin(); }
  std::vector<Ref<Layer>>::iterator end() { return m_layers_.end(); }
  std::vector<Ref<Layer>>::reverse_iterator rbegin() {
    return m_layers_.rbegin();
  }
  std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers_.rend(); }

 private:
  std::vector<Ref<Layer>> m_layers_;
  unsigned int m_Layer_insert_index_ = 0;
};

}  // namespace xuzy::Window