#pragma once

#include <glog/logging.h>

#include <vector>

#include "stdint.h"
#include "stdio.h"

/**
 * Data Source: THE MNIST DATABASE of handwritten digits
 * http://yann.lecun.com/exdb/mnist/
 */

namespace xuzy::ML::Data {

/**
 * @brief Data Container for test sets, including feature vector, label, etc.
 * typename T: type of feature
 * typename L: type of label
 */
template <typename T, typename L>
class Data {
 public:
  Data() { feature_vector_ = new std::vector<T>; }

  ~Data() {
    delete feature_vector_;
    delete class_vector_;
  }

  void set_feature_vector(std::vector<T>* p_vector) {
    feature_vector_ = p_vector;
  }

  void append_to_feature_vector(T p_value) {
    feature_vector_->push_back(p_value);
  };

  void set_class_vector(int p_count) {
    class_vector_ = new std::vector<int>();

    for (int i = 0; i < p_count; i++) {
      if (i == enum_label_)
        class_vector_->push_back(1);
      else
        class_vector_->push_back(0);
    }
  }

  void set_label(L p_value) { label_ = p_value; }

  void set_enumerated_label(int p_value) { enum_label_ = p_value; }

  void set_distance(double p_value) { distance_ = p_value; }

  int get_feature_vector_size() { return feature_vector_->size(); }

  std::vector<T>* get_feature_vector() { return feature_vector_; }

  std::vector<int>* get_class_vector() { return class_vector_; }

  L get_label() { return label_; }

  int get_enumerated_label() { return enum_label_; }

  double get_distance() { return distance_; }

 private:
  std::vector<T>* feature_vector_;  // data without label
  std::vector<int>* class_vector_;
  L label_;   // label for this data
  int enum_label_;  // e.g. A->1, B->2

  double distance_;  // temp calculate result
};

}  // namespace xuzy::ML::Data