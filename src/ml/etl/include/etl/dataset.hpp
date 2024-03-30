#pragma once

#include <vector>

#include "etl/data.hpp"

namespace xuzy::ML::ETL {

template <typename T, typename L>
class DataSet {
 protected:
  std::vector<Data<T, L>*>* training_data_;
  std::vector<Data<T, L>*>* test_data_;
  std::vector<Data<T, L>*>* validation_data_;

 public:
  DataSet();
  ~DataSet();

  void set_training_data(std::vector<Data<T, L>*>* data);
  void set_test_data(std::vector<Data<T, L>*>* data);
  void set_validation_data(std::vector<Data<T, L>*>* data);

  std::vector<Data<T, L>*>* get_training_data();
  std::vector<Data<T, L>*>* get_test_data();
  std::vector<Data<T, L>*>* get_validation_data();

  int get_training_data_size();
  int get_test_data_size();
  int get_validation_data_size();
};

template <typename T, typename L>
DataSet<T, L>::DataSet() {
  training_data_ = new std::vector<Data<T, L>*>;
  validation_data_ = new std::vector<Data<T, L>*>;
  test_data_ = new std::vector<Data<T, L>*>;
}

template <typename T, typename L>
DataSet<T, L>::~DataSet() {
  // Free dynamic allocated stuff
  if (training_data_) {
    for (Data<T, L>* data : *training_data_) {
      delete data;
    }
    training_data_->clear();
    delete training_data_;
  }

  if (validation_data_) {
    for (Data<T, L>* data : *validation_data_) {
      delete data;
    }
    validation_data_->clear();
    delete validation_data_;
  }

  if (test_data_) {
    for (Data<T, L>* data : *test_data_) {
      delete data;
    }
    test_data_->clear();
    delete test_data_;
  }
}

template <typename T, typename L>
void DataSet<T, L>::set_training_data(std::vector<Data<T, L>*>* data) {
  training_data_ = data;
}

template <typename T, typename L>
void DataSet<T, L>::set_test_data(std::vector<Data<T, L>*>* data) {
  test_data_ = data;
}

template <typename T, typename L>
void DataSet<T, L>::set_validation_data(std::vector<Data<T, L>*>* data) {
  validation_data_ = data;
}

template <typename T, typename L>
std::vector<Data<T, L>*>* DataSet<T, L>::get_training_data() {
  return training_data_;
}

template <typename T, typename L>
std::vector<Data<T, L>*>* DataSet<T, L>::get_test_data() {
  return test_data_;
}

template <typename T, typename L>
std::vector<Data<T, L>*>* DataSet<T, L>::get_validation_data() {
  return validation_data_;
}

template <typename T, typename L>
int DataSet<T, L>::get_training_data_size() {
  return training_data_->size();
}

template <typename T, typename L>
int DataSet<T, L>::get_test_data_size() {
  return test_data_->size();
}

template <typename T, typename L>
int DataSet<T, L>::get_validation_data_size() {
  return validation_data_->size();
}

}  // namespace xuzy::ML::ETL