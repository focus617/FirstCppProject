#include "neural_network/network.hpp"

#include <glog/logging.h>

#include <algorithm>
#include <numeric>

namespace xuzy::ML::CLASSIFIER {

Network::Network(std::vector<int> p_layers_spec, int p_input_size,
                 int p_num_classes, double p_learning_rate)
    : learning_rate_(p_learning_rate) {
  if (p_input_size <= 0) {
    LOG(WARNING) << "Wrong Parameter InputSize <= 0 (value = " << p_input_size
                 << ")";
    exit(1);
  }
  if (p_num_classes <= 0) {
    LOG(WARNING) << "Wrong Parameter NumClasses <= 0 (value = " << p_num_classes
                 << ")";
    exit(1);
  }

  for (size_t i = 0; i < p_layers_spec.size(); i++) {
    if (i == 0) {
      // Input Layer
      layers_.push_back(new Layer(p_input_size, p_layers_spec.at(i)));
    } else {
      // Hidden Layers in middle
      layers_.push_back(
          new Layer(layers_.at(i - 1)->neurons.size(), p_layers_spec.at(i)));
    }
  }

  // Output Layer
  layers_.push_back(
      new Layer(layers_.at(layers_.size() - 1)->neurons.size(), p_num_classes));
}

Network::~Network() {
  for (Layer* layer : layers_) {
    delete layer;
  }
  layers_.clear();
}

double Network::activate(std::vector<double> p_weights,
                         std::vector<double> p_inputs) {
  double activation = p_weights.back();  // bias term
  for (size_t i = 0; i < p_weights.size() - 1; i++) {
    activation += p_weights[i] * p_inputs[i];
  }
  return activation;
}

double Network::transfer(double activation) {
  return 1.0 / (1.0 + exp(-activation));
}

double Network::transfer_derivative(double p_output) {
  return p_output * (1 - p_output);
}

std::vector<double> Network::fprop(ETL::Data<double, std::string>* data) {
  std::vector<double> inputs = *(data->get_normalized_feature_vector());

  for (size_t i = 0; i < layers_.size(); i++) {
    std::vector<double> new_inputs;

    Layer* layer = layers_.at(i);
    for (Neuron* n : layer->neurons) {
      double activation = this->activate(n->get_weights(), inputs);
      n->output = this->transfer(activation);
      new_inputs.push_back(n->output);
    }
    inputs = new_inputs;
  }
  return inputs;  // outputs of output layer
}

void Network::bprop(ETL::Data<double, std::string>* data) {
  for (size_t i = layers_.size() - 1; i > 0; i--) {
    std::vector<double> errors;

    Layer* layer = layers_.at(i);
    if (i != layers_.size() - 1) {
      // Layers not output layer
      for (size_t j = 0; j < layer->neurons.size(); j++) {
        double error = 0.0;
        for (Neuron* n : layers_.at(i + 1)->neurons) {
          error += n->get_weights().at(j) * n->delta;
        }
        errors.push_back(error);
      }
    } else {
      // Output layer
      for (size_t j = 0; j < layer->neurons.size(); j++) {
        Neuron* n = layer->neurons.at(j);
        errors.push_back(  // expected - actual
            (double)data->get_class_vector()->at(j) - n->output);
      }
    }
    // Calculate the delta of each neuron in current layer
    for (size_t j = 0; j < layer->neurons.size(); j++) {
      Neuron* n = layer->neurons.at(j);
      n->delta =  // gradient / derivative part of back prop.
          errors.at(j) * this->transfer_derivative(n->output);
    }
  }
}

void Network::update_weigths(ETL::Data<double, std::string>* data) {
  std::vector<double> inputs = *(data->get_normalized_feature_vector());

  for (size_t i = 0; i < layers_.size(); i++) {
    if (i != 0) {
      for (Neuron* n : layers_.at(i - 1)->neurons) {
        inputs.push_back(n->output);
      }
    }
    for (Neuron* n : layers_.at(i)->neurons) {
      for (size_t j = 0; j < inputs.size(); j++) {
        n->get_weights().at(j) +=
            this->learning_rate_ * n->delta * inputs.at(j);
      }
      n->get_weights().back() += this->learning_rate_ * n->delta;
    }
    inputs.clear();
  }
}

int Network::predict(ETL::Data<double, std::string>* data) {
  std::vector<double> outputs = fprop(data);
  return std::distance(outputs.begin(),
                       std::max_element(outputs.begin(), outputs.end()));
}

void Network::train(int p_num_epochs) {
  for (int i = 0; i < p_num_epochs; i++) {
    double sum_error = 0.0;

    for (ETL::Data<double, std::string>* data : *this->training_data_) {
      std::vector<double> outputs = fprop(data);
      std::vector<int> expected = *data->get_class_vector();

      double temp_error_sum = 0.0;
      for (size_t j = 0; j < outputs.size(); j++) {
        temp_error_sum += pow((double)expected.at(j) - outputs.at(j), 2);
      }
      sum_error += temp_error_sum;

      bprop(data);
      update_weigths(data);
    }
    LOG(INFO) << "Iteration: " << i << "\t Error= " << sum_error;
  }
}

double Network::test() {
  double num_correct = 0.0;
  double count = 0.0;

  for (ETL::Data<double, std::string>* data : *this->test_data_) {
    ++count;

    int index = predict(data);
    if (data->get_class_vector()->at(index) == 1) ++num_correct;
  }
  test_performance = num_correct / count;
  return test_performance;
}

void Network::validate() {
  double num_correct = 0.0;
  double count = 0.0;

  for (ETL::Data<double, std::string>* data : *this->validation_data_) {
    ++count;

    int index = predict(data);
    if (data->get_class_vector()->at(index) == 1) ++num_correct;
  }
  LOG(INFO) << "Validation Performance: " << 100 * num_correct / count << " %";
}

}  // namespace xuzy::ML::CLASSIFIER