#pragma once

#include <vector>

#include "layer.hpp"

namespace xuzy {

class XUZY_API LayerStack {
 public:
  LayerStack();
  ~LayerStack();

  // Overlayer is on the back, which will process event at first
  void push_layer(Layer* layer);
  void push_overlay(Layer* overlay);
  void pop_layer(Layer* layer);
  void pop_overlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return m_layers_.begin(); }
  std::vector<Layer*>::iterator end() { return m_layers_.end(); }

 private:
  std::vector<Layer*> m_layers_;
  std::vector<Layer*>::iterator m_Layer_insert_;
};

}  // namespace xuzy