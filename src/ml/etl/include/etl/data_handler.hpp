#pragma once

#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "etl/data.hpp"
#include "etl/dataset.hpp"
#include "stdint.h"

namespace xuzy::ML::ETL {

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

  void read_input_data(std::string path);
  void read_labels_data(std::string path);
  void read_dataset_from_csv(std::string path, std::string delimiter);

  void split_data(DataSet<T, L>* dataset);

  void normalize();
  void init_classes_vector();

  std::map<L, int> get_class_map();

  int get_class_counts();
  int get_dataset_size();

  void dump_datasets(unsigned precision);

 private:
  uint32_t convert_to_little_endian(const unsigned char* bytes);
  void move_data(std::vector<Data<T, L>*>* candidate,
                 std::vector<Data<T, L>*>* output_dataset, int size);

 public:
  const double TRAIN_SET_PERCENT = 0.75;
  const double TEST_SET_PERCENT = 0.20;
  const double VALIDATION_PERCENT = 0.05;

 private:
  // all of the data(pre-split)
  std::vector<Data<T, L>*>* data_array_;
  // map of label to enum label
  std::map<L, int> class_map_;
  // total number of classification
  int class_counts_;
};

}  // namespace xuzy::ML::ETL

#include "etl/data_handler.inl"
