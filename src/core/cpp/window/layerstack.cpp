#include "pch.h"

#include "window/layerstack.hpp"

namespace xuzy {

LayerStack::~LayerStack() {
  for (Ref<Layer> layer : m_layers_) {
    layer->on_detach();
  }
  m_layers_.clear();
}

void LayerStack::push_layer(const Ref<Layer>& layer) {
  m_layers_.emplace(m_layers_.begin() + m_Layer_insert_index_, layer);
  m_Layer_insert_index_++;
  layer->on_attach();
}

void LayerStack::pop_layer(const Ref<Layer>& layer) {
  auto it = std::find(m_layers_.begin(),
                      m_layers_.begin() + m_Layer_insert_index_, layer);
  if (it != m_layers_.begin() + m_Layer_insert_index_) {
    layer->on_detach();
    m_layers_.erase(it);
    m_Layer_insert_index_--;
  }
}

void LayerStack::push_overlay(const Ref<Layer>& overlay) {
  m_layers_.emplace_back(overlay);
  overlay->on_attach();
}

void LayerStack::pop_overlay(const Ref<Layer>& overlay) {
  auto it = std::find(m_layers_.begin() + m_Layer_insert_index_,
                      m_layers_.end(), overlay);
  if (it != m_layers_.end()) {
    overlay->on_detach();
    m_layers_.erase(it);
  }
}

}  // namespace xuzy