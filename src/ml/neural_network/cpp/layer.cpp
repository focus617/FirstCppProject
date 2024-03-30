#include "neural_network/layer.hpp"

namespace xuzy::ML::CLASSIFIER {

Layer::Layer(int previous_layer_size, int current_layer_size) {
  for (int i = 0; i < current_layer_size; i++) {
    neurons.push_back(new Neuron(previous_layer_size, current_layer_size));
  }
  this->size_ = current_layer_size;
}

Layer::~Layer(){
  neurons.clear();
}

}  // namespace xuzy::ML::CLASSIFIER