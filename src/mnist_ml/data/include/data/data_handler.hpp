#pragma once

#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "data/data.hpp"
#include "stdint.h"

namespace xuzy::ML::Data {

/**
 * @brief implement all of data ETL (Extract, Transform, Load) operation logic,
 * e.g. reading, format converting, splitting, counting unique classes and pass
 * training data, test data and validation data, etc.
 * 
 * typename T: type of feature
 * typename L: type of label
 */
template <typename T, typename L>
class DataHandler {
 public:
  DataHandler();
  ~DataHandler();

  void read_feature_vector(std::string path);
  void read_feature_labels(std::string path);
  void read_feature_csv(std::string path, std::string delimiter);

  void split_data();
  void count_classes();

  std::vector<Data<T, L>*>* get_training_data();
  std::vector<Data<T, L>*>* get_test_data();
  std::vector<Data<T, L>*>* get_validation_data();

 private:
  uint32_t convert_to_little_endian(const unsigned char* bytes);

 private:
  // all of the data(pre-split)
  std::vector<Data<T, L>*>* data_array_;

  // data set after splitting
  std::vector<Data<T, L>*>* training_data_;
  std::vector<Data<T, L>*>* test_data_;
  std::vector<Data<T, L>*>* validation_data_;

  int num_classes_;
  int feature_vector_size_;

  std::map<L, int> class_map_;

  const double TRAIN_SET_PERCENT = 0.75;
  const double TEST_SET_PERCENT = 0.20;
  const double VALIDATION_PERCENT = 0.05;
};

}  // namespace xuzy::ML::Data

#include "data/data_handler.inl"
