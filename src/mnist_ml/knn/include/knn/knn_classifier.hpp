#pragma once

#include <vector>

#include "data/data.hpp"

namespace xuzy::ML::CLASSIFIER {

/**
 * @brief K-Nearest Neighbors (KNN) Algorithm
 * typename T: type of feature
 * typename L: type of label
 */
template <typename T, typename L>
class KnnClassifier {
 public:
  KnnClassifier(int k);
  KnnClassifier();
  ~KnnClassifier();

  void set_training_data(std::vector<Data::Data<T, L>*>* p_vector);
  void set_test_data(std::vector<Data::Data<T, L>*>* p_vector);
  void set_validation_data(std::vector<Data::Data<T, L>*>* p_vector);
  void set_k(int p_value);

  double validate_performance();
  double test_performance();

 private:
  double performance(std::vector<Data::Data<T, L>*>* p_data_set,
                     bool show_detail = false);

  void find_knearest(Data::Data<T, L>* p_query_point);

  double calculate_distance(Data::Data<T, L>* p_query_point,
                            Data::Data<T, L>* p_input);

  L predict();

 private:
  int k_;
  std::vector<Data::Data<T, L>*>* training_data_;
  std::vector<Data::Data<T, L>*>* test_data_;
  std::vector<Data::Data<T, L>*>* validation_data_;

  std::vector<Data::Data<T, L>*>* neighbors_ = nullptr;
};

}  // namespace xuzy::ML::CLASSIFIER

#include "knn/knn_classifier.inl"