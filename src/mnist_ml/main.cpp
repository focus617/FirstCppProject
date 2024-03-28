#include <glog/logging.h>

#include "data/data_handler.hpp"
#include "knn/knn_classifier.hpp"
#include "neural_network/network.hpp"

void init_logger(const char* argv0) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv0);
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
}

void close_logger() {
  // Shutdown Google’s logging system.
  google::ShutdownGoogleLogging();
}

int main(int argc, char* argv[]) {
  // Initialize Google’s logging library.
  init_logger(argv[0]);

#ifdef MNIST
  auto dh = new xuzy::ML::Data::DataHandler<uint8_t, uint8_t>();
  dh->read_input_data("../data/train-images-idx3-ubyte");
  dh->read_labels_data("../data/train-labels-idx1-ubyte");
  dh->count_classes();
#else
  auto dh = new xuzy::ML::Data::DataHandler<double, std::string>();
  dh->read_dataset_in_csv("../data/iris.data", ",");
#endif

  dh->split_data();

#if KNN

  auto knn_classifier =
      new xuzy::ML::CLASSIFIER::KnnClassifier<uint8_t, uint8_t>();
  knn_classifier->set_training_data(dh->get_training_data());
  knn_classifier->set_test_data(dh->get_test_data());
  knn_classifier->set_validation_data(dh->get_validation_data());

  double best_performance = 0;
  int best_k = 1;

  for (int i = 1; i <= 4; i++) {
    knn_classifier->set_k(i);
    double performance = knn_classifier->validate_performance();
    LOG(INFO) << "Validation Performance(for K = " << i << ") = " << performance
              << " %";

    if (i == 1) {
      best_performance = performance;
    } else {
      if (performance > best_performance) {
        best_performance = performance;
        best_k = i;
      }
    }
  }

  knn_classifier->set_k(best_k);
  double performance = knn_classifier->test_performance();
  LOG(INFO) << "Test Performance(for best K = " << best_k
            << ") = " << performance << " %";

#endif

#if NEURAL_NETWORK
  int input_size =
      dh->get_training_data()->at(0)->get_normalized_feature_vector()->size();
  int num_classes = dh->get_class_counts();

  LOG(INFO) << "Create NEURAL_NETWORK with InputSize = " << input_size
            << ", NumClasses = " << num_classes;
  
  std::vector<int> hidden_layers = {10};

  auto lambda = [&]() {
    auto net = new xuzy::ML::CLASSIFIER::Network(
        hidden_layers,
        dh->get_training_data()->at(0)->get_normalized_feature_vector()->size(),
        dh->get_class_counts(), 0.25);
    net->set_training_data(dh->get_training_data());
    net->set_test_data(dh->get_test_data());
    net->set_validation_data(dh->get_validation_data());
    net->train(15);
    net->validate();

    LOG(INFO) << "Test Performance: " << net->test();

    delete net;
  };

  lambda();

#endif

  close_logger();

  return EXIT_SUCCESS;
}