#pragma once

#include "etl/dataset.hpp"

namespace xuzy::ML::CLASSIFIER {

/**
 * @brief K-Nearest Neighbors (KNN) Algorithm
 * typename T: type of feature
 * typename L: type of label
 */
template <typename T, typename L>
class KnnClassifier : public ETL::DataSet<T, L> {
 public:
  KnnClassifier(int k);
  KnnClassifier();
  ~KnnClassifier();

  void set_k(int p_value);

  double validate_performance();
  double test_performance();

 private:
  double performance(std::vector<ETL::Data<T, L>*>* p_data_set,
                     bool show_detail = false);

  void find_knearest(ETL::Data<T, L>* p_query_point);

  double calculate_distance(ETL::Data<T, L>* p_query_point,
                            ETL::Data<T, L>* p_input);

  L predict();

 private:
  int k_;
  std::vector<ETL::Data<T, L>*>* neighbors_;
};

}  // namespace xuzy::ML::CLASSIFIER

#include "knn/knn_classifier.inl"