#pragma once

#include <vector>

#include "data/data.hpp"

namespace xuzy::ML::Data {

template <typename T, typename L>
class CommonData {
 protected:
  std::vector<Data<T, L>*>* training_data_;
  std::vector<Data<T, L>*>* test_data_;
  std::vector<Data<T, L>*>* validation_data_;

 public:
  void set_training_data(std::vector<Data<T, L>*>* data);
  void set_test_data(std::vector<Data<T, L>*>* data);
  void set_validation_data(std::vector<Data<T, L>*>* data);
};

template <typename T, typename L>
void CommonData<T, L>::set_training_data(std::vector<Data<T, L>*>* data) {
  training_data_ = data;
}

template <typename T, typename L>
void CommonData<T, L>::set_test_data(std::vector<Data<T, L>*>* data) {
  test_data_ = data;
}

template <typename T, typename L>
void CommonData<T, L>::set_validation_data(
    std::vector<Data<T, L>*>* data) {
  validation_data_ = data;
}

}  // namespace xuzy::ML::Data