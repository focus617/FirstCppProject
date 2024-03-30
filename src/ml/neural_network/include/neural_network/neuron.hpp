#pragma once

#include <cmath>
#include <vector>

namespace xuzy::ML::CLASSIFIER {

class Neuron {
 public:
  /**
   * @brief Constructor
   * @param
   * @param
   */
  Neuron(int previous_layer_size, int current_layer_size);

  /**
   * @brief Destructor
   */
  ~Neuron();

  std::vector<double> get_weights() { return weights_; }

 public:
  double output;
  double delta;

 private:
  double generate_random_number(double min, double max);
  void initialize_weights(int previous_layer_size);

 private:
  std::vector<double> weights_;
};

}  // namespace xuzy::ML::CLASSIFIER