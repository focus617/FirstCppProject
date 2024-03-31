#pragma once

#include <cmath>
#include <vector>

#include "etl/dataset.hpp"

namespace xuzy::ML::CLASSIFIER {

class Perceptron {
  const double BIAS_INPUT = 1;
  const double MAX_EPOCHS = 100;

 public:
  /**
   * @brief Constructor
   */
  Perceptron(int p_input_size);

  /**
   * @brief Destructor
   */
  ~Perceptron();

  /**
   * @brief Destructor
   */
  void set_dataset(ETL::DataSet<double, std::string>* p_dataset);

  void train();

  int classify(ETL::Data<double, std::string>* p_data);

 private:
  void initialize_weights(int p_weight_size);

  /**
   * @brief
   * @param p_data, input feature vector
   */
  double integrate(ETL::Data<double, std::string>* p_data);

  /**
   * @brief
   * @param p_input, output from integration
   */
  int activate(const double p_input);

 private:
  ETL::DataSet<double, std::string>* dataset_;

  std::vector<double> weights_;
};

}  // namespace xuzy::ML::CLASSIFIER