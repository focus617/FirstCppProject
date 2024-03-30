#pragma once

#include "etl/dataset.hpp"
#include "neural_network/layer.hpp"
#include "neural_network/neuron.hpp"

namespace xuzy::ML::CLASSIFIER {

class Network : public ETL::DataSet<double, std::string> {
 public:
  /**
   * @brief Constructor
   * @param p_layers_spec, num of neural for each layer
   * @param p_input_size, size of input data set
   * @param p_num_classes
   * @param p_learning_rate
   */
  Network(std::vector<int> p_layers_spec, int p_input_size, int p_num_classes,
          double p_learning_rate);

  /**
   * @brief Destructor
   */
  ~Network();

  /**
   * @brief
   * @param p_weights, weight vector of neural
   * @param p_inputs, input from previous layer or input itself
   */
  double activate(std::vector<double> p_weights, std::vector<double> p_inputs);

  /**
   * @brief
   * @param activation
   */
  double transfer(double activation);

  /**
   * @brief Used for backprop
   * @param p_output
   */
  double transfer_derivative(double p_output);

  /**
   * @brief operation for training
   * @param data Input data set
   * @returns last layer output
   */
  std::vector<double> fprop(ETL::Data<double, std::string>* data);
  void bprop(ETL::Data<double, std::string>* data);
  void update_weigths(ETL::Data<double, std::string>* data);

  /**
   * @brief Returns the index of the maximun value in the output array
   * @param
   */
  int predict(ETL::Data<double, std::string>* data);

  /**
   * @brief
   * @param p_num_epochs, num iterations for training
   */
  void train(int p_num_epochs);

  /**
   * @brief
   */
  double test();

  /**
   * @brief
   */
  void validate();

 private:
  std::vector<Layer*> layers_;

  double learning_rate_;  // learning rate
  double test_performance;
};

}  // namespace xuzy::ML::CLASSIFIER