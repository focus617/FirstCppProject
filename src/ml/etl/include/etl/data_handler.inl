#include <glog/logging.h>
namespace xuzy::ML::ETL {

template <typename T, typename L>
DataHandler<T, L>::DataHandler() : class_counts_(0) {
  data_array_ = new std::vector<Data<T, L>*>();
}

template <typename T, typename L>
DataHandler<T, L>::~DataHandler() {
  // Free dynamic allocated stuff
  if (data_array_) {
    for (Data<T, L>* data : *data_array_) {
      delete data;
    }
    data_array_->clear();
    delete data_array_;
  }
}

template <typename T, typename L>
void DataHandler<T, L>::read_input_data(std::string path) {
  uint32_t header[4];  // [MAGIC | NUM_IMAGS | ROWSIZE | COLSIZE]
  unsigned char bytes[4];

  FILE* f = fopen(path.c_str(), "r");
  if (f) {
    for (int i = 0; i < 4; i++) {
      if (fread(bytes, sizeof(bytes), 1, f)) {
        header[i] = convert_to_little_endian(bytes);
      }
    }
    LOG(INFO) << "Done getting Input File Header.";

    uint32_t image_size = header[2] * header[3];
    for (uint32_t i = 0; i < header[1]; i++) {
      Data<T, L>* data = new Data<T, L>();

      uint8_t element[1];
      for (uint32_t j = 0; j < image_size; j++) {
        if (fread(element, sizeof(element), 1, f)) {
          data->append_to_feature_vector(element[0]);
        } else {
          LOG(WARNING) << "Error Reading from file(" << path << ").";
          exit(1);
        }
      }
      data_array_->push_back(data);
    }
    LOG(INFO) << "Successfully read and stored " << data_array_->size()
              << " feature vectors.";
  } else {
    LOG(WARNING) << "Could not find file(" << path << ").";
    exit(1);
  }
}

template <typename T, typename L>
void DataHandler<T, L>::read_labels_data(std::string path) {
  uint32_t header[2];  // [MAGIC | NUM_LABELS]
  unsigned char bytes[4];

  FILE* f = fopen(path.c_str(), "r");
  if (f) {
    for (int i = 0; i < 2; i++) {
      if (fread(bytes, sizeof(bytes), 1, f)) {
        header[i] = convert_to_little_endian(bytes);
      }
    }

    if (data_array_->size() != header[1]) {
      LOG(WARNING) << "Size Mismatch between label set and feature set!";
      exit(1);
    }

    LOG(INFO) << "Done getting Label File Header.";

    class_counts_ = 0;
    for (uint32_t i = 0; i < header[1]; i++) {
      uint8_t element[1];
      if (fread(element, sizeof(element), 1, f)) {
        if (class_map_.find(element[0]) == class_map_.end()) {
          // if new label
          class_map_[element[0]] = class_counts_;
          ++class_counts_;
        }
        data_array_->at(i)->set_label(element[0]);
        data_array_->at(i)->set_enumerated_label(class_map_[element[0]]);
      } else {
        LOG(WARNING) << "Error Reading from file(" << path << ").";
        exit(1);
      }
    }
    LOG(INFO) << "Successfully read and stored " << data_array_->size()
              << " labels, with " << class_counts_ << " unique classes.";
  } else {
    LOG(WARNING) << "Could not find file(" << path << ").";
    exit(1);
  }
}

template <typename T, typename L>
void DataHandler<T, L>::read_dataset_from_csv(std::string path,
                                              std::string delimiter) {
  class_counts_ = 0;
  std::ifstream dataset_file(path.c_str());
  std::string line;  // holds each line

  // Each line is a sample data with feature vector and label
  while (std::getline(dataset_file, line)) {
    if (line.length() == 0) continue;

    size_t position = 0;
    std::string token;  // value between delimiter
    Data<T, L>* data = new Data<T, L>();

    // Extract Feature
    while ((position = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, position);
      data->append_to_feature_vector(std::stod(token));
      line.erase(0, position + delimiter.length());
    }
    // Extract Label
    data->set_label(line);

    // Setup enumerated_label
    if (class_map_.find(line) == class_map_.end()) {
      // if new label
      class_map_[line] = class_counts_;
      ++class_counts_;
    }
    data->set_enumerated_label(class_map_[line]);

    // Append new data to dataset
    data_array_->push_back(data);
  }

  LOG(INFO) << "Successfully read and stored " << data_array_->size()
            << " labels, with " << class_counts_ << " unique classes.";

  for (Data<T, L>* data : *data_array_) {
    data->set_class_vector(class_counts_);
  }

  normalize();
}

template <typename T, typename L>
void DataHandler<T, L>::split_data(DataSet<T, L>* dataset) {
  int train_size = data_array_->size() * TRAIN_SET_PERCENT;
  int test_size = data_array_->size() * TEST_SET_PERCENT;
  int valid_size = data_array_->size() * VALIDATION_PERCENT;

  // Training DataSet
  move_data(data_array_, dataset->get_training_data(), train_size);

  // Test DataSet
  move_data(data_array_, dataset->get_test_data(), test_size);

  // Validation DataSet
  move_data(data_array_, dataset->get_validation_data(), valid_size);

  LOG(INFO) << "Training DataSet Size: " << dataset->get_training_data_size();
  LOG(INFO) << "Test DataSet Size: " << dataset->get_test_data_size();
  LOG(INFO) << "Validation DataSet Size: "
            << dataset->get_validation_data_size();
}

template <typename T, typename L>
void DataHandler<T, L>::move_data(std::vector<Data<T, L>*>* candidate,
                                  std::vector<Data<T, L>*>* output_dataset,
                                  int size) {
  int count = 0;
  while (count < size) {
    int random_index = rand() % candidate->size();  // [0, candidate->size()-1]
    output_dataset->push_back(candidate->at(random_index));
    candidate->erase(candidate->begin() + random_index);
    ++count;
  }
}

template <typename T, typename L>
void DataHandler<T, L>::normalize() {
  std::vector<T> mins, maxs;
  Data<T, L>* data = data_array_->at(0);

  // fill min and max lists with first feature vector
  for (auto val : *data->get_feature_vector()) {
    mins.push_back(val);
    maxs.push_back(val);
  }
  // find min and max value from rest fv
  for (size_t i = 1; i < data_array_->size(); i++) {
    data = data_array_->at(i);
    for (int j = 0; j < data->get_feature_vector_dimension(); j++) {
      double value = (double)data->get_feature_vector()->at(j);
      if (value < mins.at(j)) mins[j] = value;
      if (value > maxs.at(j)) maxs[j] = value;
    }
  }

  // normalize data array
  for (size_t i = 0; i < data_array_->size(); i++) {
    data_array_->at(i)->set_normalized_feature_vector(new std::vector<T>());
    data_array_->at(i)->set_class_vector(class_counts_);

    for (int j = 0; j < data->get_feature_vector_dimension(); j++) {
      if (maxs[j] - mins[j] < 0.0000001)
        data_array_->at(i)->append_to_normalized_feature_vector(0.0);
      else
        data_array_->at(i)->append_to_normalized_feature_vector(
            (double)(data_array_->at(i)->get_feature_vector()->at(j) -
                     mins[j]) /
            (maxs[j] - mins[j]));
    }
  }
}

template <typename T, typename L>
void DataHandler<T, L>::dump_datasets(unsigned precision) {
  for (Data<T, L>* data : *data_array_) data->dump_data(precision);
}

template <typename T, typename L>
void DataHandler<T, L>::init_classes_vector() {
  for (Data<T, L>* data : *data_array_) data->set_class_vector(class_counts_);
}

template <typename T, typename L>
int DataHandler<T, L>::get_class_counts() {
  return class_counts_;
}

template <typename T, typename L>
int DataHandler<T, L>::get_dataset_size() {
  return data_array_->size();
}

template <typename T, typename L>
uint32_t DataHandler<T, L>::convert_to_little_endian(
    const unsigned char* bytes) {
  return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) |
                    (bytes[3]));
}

}  // namespace xuzy::ML::ETL