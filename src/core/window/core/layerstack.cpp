#include "pch.h"

#include "layerstack.hpp"

namespace xuzy::Window {

LayerStack::~LayerStack() {
  for (Ref<ALayer> layer : m_layers_) {
    layer->on_detach();
  }
  m_layers_.clear();
}

void LayerStack::push_layer(const Ref<ALayer>& layer) {
  m_layers_.emplace(m_layers_.begin() + m_Layer_insert_index_, layer);
  m_Layer_insert_index_++;
  layer->on_attach();
}

void LayerStack::pop_layer(const Ref<ALayer>& layer) {
  auto it = std::find(m_layers_.begin(),
                      m_layers_.begin() + m_Layer_insert_index_, layer);
  if (it != m_layers_.begin() + m_Layer_insert_index_) {
    layer->on_detach();
    m_layers_.erase(it);
    m_Layer_insert_index_--;
  }
}

void LayerStack::push_overlay(const Ref<ALayer>& overlay) {
  m_layers_.emplace_back(overlay);
  overlay->on_attach();
}

void LayerStack::pop_overlay(const Ref<ALayer>& overlay) {
  auto it = std::find(m_layers_.begin() + m_Layer_insert_index_,
                      m_layers_.end(), overlay);
  if (it != m_layers_.end()) {
    overlay->on_detach();
    m_layers_.erase(it);
  }
}

void LayerStack::on_draw(){
  for (auto it = begin(); it != end(); ++it) {
      (*it)->on_draw();
  }
}

}  // namespace xuzy::Window