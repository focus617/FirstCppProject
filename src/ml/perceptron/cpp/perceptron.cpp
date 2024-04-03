#include "perceptron/perceptron.hpp"

#include <algorithm>
#include <numeric>
#include <random>

namespace xuzy::ML::CLASSIFIER {

Perceptron::Perceptron(int p_input_size) : dataset_(nullptr) {
  // more dimension w0: weights_.back()
  initialize_weights(p_input_size + 1);
}

Perceptron::~Perceptron() {
  if (dataset_) delete dataset_;
}

void Perceptron::set_dataset(ETL::DataSet<double, std::string>* p_dataset) {
  dataset_ = p_dataset;
}

void Perceptron::initialize_weights(int p_weight_size) {
  std::random_device seed;
  std::default_random_engine generator(seed());
  std::uniform_real_distribution<double> distributionDouble(-.5, .5);

  for (int i = 0; i < p_weight_size; i++) {
    weights_.push_back(distributionDouble(generator));
  }
}

double Perceptron::integrate(ETL::Data<double, std::string>* p_data) {
  std::vector<double> inputs = *(p_data->get_feature_vector());
  /*
  double sum = 0;
  for (int i = 0; i < inputs.size() - 1; i++) {
      sum += inputs[i] * weights_[i];
  }
  return sum;
  */
  return inner_product(inputs.begin(), inputs.end() - 1, weights_.begin(), 0.0);
}

int Perceptron::activate(const double p_input) {
  return (p_input >= 0) ? 1 : 0;
}

void Perceptron::train() {
  if (nullptr == dataset_) {
    LOG(ERROR) << "DataSet Empty.";
    exit(1);
  }

  std::vector<ETL::Data<double, std::string>*>* training_dataset =
      dataset_->get_training_data();

  if (training_dataset->size() == 0) {
    LOG(ERROR) << "Training DataSet Empty.";
    exit(1);
  } else {
    LOG(INFO) << "Training DataSet Size: " << training_dataset->size();
  }

  for (int i = 0; i < MAX_EPOCHS; i++) {
    int incorrect = 0;

    std::random_device seed;
    std::shuffle(training_dataset->begin(), training_dataset->end(), seed);

    for (ETL::Data<double, std::string>* data : *training_dataset) {
      if ((data->get_enumerated_label() != 0) &&
          (data->get_enumerated_label() != 1)) {
        continue;
      }

      double dot = integrate(data);
      dot += BIAS_INPUT * weights_.back();
      int calculated_output = activate(dot);

      // Perform training
      if ((data->get_enumerated_label() == 1) && (calculated_output == 0)) {
        incorrect++;
        for (size_t k = 0; k < weights_.size() - 1; k++) {
          weights_[k] += data->get_feature_vector()->at(k);
        }
        // weights_.back() += BIAS_INPUT * (double)error;
      } else if ((data->get_enumerated_label() == 0) &&
                 (calculated_output == 1)) {
        incorrect++;
        for (size_t k = 0; k < weights_.size() - 1; k++) {
          weights_[k] -= data->get_feature_vector()->at(k);
        }
        // weights_.back() += BIAS_INPUT * (double)error;
      }
    }

    LOG(INFO) << "EPOCHS: " << i << "\t Incorrect = " << incorrect;
    if (incorrect == 0) {
      LOG(INFO) << "Final Weight Values : [";
      for (double w : weights_) {
        LOG(INFO) << "\t" << w << ", ";
      }
      LOG(INFO) << "\t]";
      break;
    }
  }
}

int Perceptron::classify(ETL::Data<double, std::string>* p_data) {
  double dot = integrate(p_data);
  dot += BIAS_INPUT * weights_.back();
  return activate(dot);
}

}  // namespace xuzy::ML::CLASSIFIER