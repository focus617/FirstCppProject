#include "pch.h"

#include "window/layerstack.hpp"

namespace xuzy {

LayerStack::LayerStack() { m_Layer_insert_ = m_layers_.begin(); }

LayerStack::~LayerStack() {
  for (Layer* layer : m_layers_) delete layer;
}

void LayerStack::push_layer(Layer* layer) {
  m_Layer_insert_ = m_layers_.emplace(m_Layer_insert_, layer);
}

void LayerStack::push_overlay(Layer* overlay) {
  m_layers_.emplace_back(overlay);
}

void LayerStack::pop_layer(Layer* layer) {
  auto it = std::find(m_layers_.begin(), m_layers_.end(), layer);
  if (it != m_layers_.end()) {
    m_layers_.erase(it);
    m_Layer_insert_--;
  }
}

void LayerStack::pop_overlay(Layer* overlay) {
  auto it = std::find(m_layers_.begin(), m_layers_.end(), overlay);
  if (it != m_layers_.end()) m_layers_.erase(it);
}

}  // namespace xuzy