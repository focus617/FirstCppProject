#pragma once

#include <vector>

#include "neural_network/neuron.hpp"

namespace xuzy::ML::CLASSIFIER {

// static int s_layer_id = 0;

class Layer {
 public:
  std::vector<Neuron*> neurons;

  Layer(int previous_layer_size, int current_layer_size);
  ~Layer() = default;

  int get_size() { return size_; }
  void set_size(int p_size) { size_ = p_size; }

  std::vector<double> get_outputs() { return outputs_; }
  void set_outputs(std::vector<double> p_value) {
    outputs_ = p_value;
  }

 private:
  int size_;
  std::vector<double> outputs_;
};

}  // namespace xuzy::ML::CLASSIFIER