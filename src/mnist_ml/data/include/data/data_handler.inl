#include <glog/logging.h>
namespace xuzy::ML::Data {

template <typename T, typename L>
DataHandler<T, L>::DataHandler() : class_counts_(0), feature_vector_size_(0) {
  data_array_ = new std::vector<Data<T, L>*>;
  training_data_ = new std::vector<Data<T, L>*>;
  validation_data_ = new std::vector<Data<T, L>*>;
  test_data_ = new std::vector<Data<T, L>*>;
}

template <typename T, typename L>
DataHandler<T, L>::~DataHandler() {
  // Free dynamic allocated stuff
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
      Data<T, L>* dt = new Data<T, L>();

      uint8_t element[1];
      for (uint32_t j = 0; j < image_size; j++) {
        if (fread(element, sizeof(element), 1, f)) {
          dt->append_to_feature_vector(element[0]);
        } else {
          LOG(WARNING) << "Error Reading from file(" << path << ").";
          exit(1);
        }
      }
      data_array_->push_back(dt);
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

    for (uint32_t i = 0; i < header[1]; i++) {
      uint8_t element[1];
      if (fread(element, sizeof(element), 1, f)) {
        data_array_->at(i)->set_label(element[0]);
      } else {
        LOG(WARNING) << "Error Reading from file(" << path << ").";
        exit(1);
      }
    }
    LOG(INFO) << "Successfully read and stored " << data_array_->size()
              << " labels.";
  } else {
    LOG(WARNING) << "Could not find file(" << path << ").";
    exit(1);
  }
}

template <typename T, typename L>
void DataHandler<T, L>::read_dataset_in_csv(std::string path,
                                            std::string delimiter) {
  class_counts_ = 0;
  std::ifstream data_file(path.c_str());
  std::string line;  // holds each line

  while (std::getline(data_file, line)) {
    if (line.length() == 0) continue;

    Data<T, L>* dt = new Data<T, L>();
    dt->set_normalized_feature_vector(new std::vector<T>());

    size_t position = 0;
    std::string token;  // value between delimiter

    // Extract Feature
    while ((position = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, position);
      dt->append_to_feature_vector(std::stod(token));
      line.erase(0, position + delimiter.length());
    }
    // Extract Label
    if (class_map_.find(line) == class_map_.end()) {
      // if new label
      class_map_[line] = class_counts_;
      ++class_counts_;
    }
    dt->set_label(line);
    dt->set_enumerated_label(class_map_[line]);
    data_array_->push_back(dt);
  }

  for (Data<T, L>* data : *data_array_) {
    data->set_class_vector(class_counts_);
  }

  normalize();
  feature_vector_size_ =
      data_array_->at(0)->get_normalized_feature_vector()->size();
}

template <typename T, typename L>
void DataHandler<T, L>::split_data() {
  std::unordered_set<int> used_indexes;

  int train_size = data_array_->size() * TRAIN_SET_PERCENT;
  int test_size = data_array_->size() * TEST_SET_PERCENT;
  int valid_size = data_array_->size() * VALIDATION_PERCENT;

  // Training Data
  int count = 0;
  while (count < train_size) {
    // [0, data_array_->size()-1]
    int random_index = rand() % data_array_->size();
    if (used_indexes.find(random_index) == used_indexes.end()) {
      training_data_->push_back(data_array_->at(random_index));
      used_indexes.insert(random_index);
      ++count;
    }
  }

  // Test Data
  count = 0;
  while (count < test_size) {
    // [0, data_array_->size()-1]
    int random_index = rand() % data_array_->size();
    if (used_indexes.find(random_index) == used_indexes.end()) {
      test_data_->push_back(data_array_->at(random_index));
      used_indexes.insert(random_index);
      ++count;
    }
  }

  // Validation Data
  count = 0;
  while (count < valid_size) {
    // [0, data_array_->size()-1]
    int random_index = rand() % data_array_->size();
    if (used_indexes.find(random_index) == used_indexes.end()) {
      validation_data_->push_back(data_array_->at(random_index));
      used_indexes.insert(random_index);
      ++count;
    }
  }

  LOG(INFO) << "Training Data Size: " << training_data_->size();
  LOG(INFO) << "Test Data Size: " << test_data_->size();
  LOG(INFO) << "Validation Data Size: " << validation_data_->size();
}

template <typename T, typename L>
void DataHandler<T, L>::count_classes() {
  int count = 0;
  for (unsigned i = 0; i < data_array_->size(); i++) {
    if (class_map_.find(data_array_->at(i)->get_label()) == class_map_.end()) {
      class_map_[data_array_->at(i)->get_label()] = count;
      data_array_->at(i)->set_enumerated_label(count);
      ++count;
    }
  }

  class_counts_ = count;
  for (Data<T, L>* data : *data_array_) data->set_class_vector(class_counts_);

  LOG(INFO) << "Successfully extracted " << class_counts_ << " unique classes.";
}

template <typename T, typename L>
void DataHandler<T, L>::normalize() {
  std::vector<T> mins, maxs;
  Data<T, L>* dt = data_array_->at(0);

  // fill min and max lists with first feature vector
  for (auto val : *dt->get_feature_vector()) {
    mins.push_back(val);
    maxs.push_back(val);
  }
  // find min and max value from rest fv
  for (size_t i = 1; i < data_array_->size(); i++) {
    dt = data_array_->at(i);
    for (int j = 0; j < dt->get_feature_vector_size(); j++) {
      double value = (double)dt->get_feature_vector()->at(j);
      if (value < mins.at(j)) mins[j] = value;
      if (value > maxs.at(j)) maxs[j] = value;
    }
  }

  // normalize data array
  for (size_t i = 0; i < data_array_->size(); i++) {
    data_array_->at(i)->set_normalized_feature_vector(new std::vector<T>());
    data_array_->at(i)->set_class_vector(class_counts_);

    for (int j = 0; j < dt->get_feature_vector_size(); j++) {
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
std::vector<Data<T, L>*>* DataHandler<T, L>::get_training_data() {
  return training_data_;
}

template <typename T, typename L>
std::vector<Data<T, L>*>* DataHandler<T, L>::get_test_data() {
  return test_data_;
}

template <typename T, typename L>
std::vector<Data<T, L>*>* DataHandler<T, L>::get_validation_data() {
  return validation_data_;
}

template <typename T, typename L>
int DataHandler<T, L>::get_class_counts() {
  return class_counts_;
}

template <typename T, typename L>
int DataHandler<T, L>::get_data_array_size() {
  return data_array_->size();
}

template <typename T, typename L>
int DataHandler<T, L>::get_training_data_size() {
  return training_data_->size();
}

template <typename T, typename L>
int DataHandler<T, L>::get_test_data_size() {
  return test_data_->size();
}

template <typename T, typename L>
int DataHandler<T, L>::get_validation_size() {
  return validation_data_->size();
}

template <typename T, typename L>
uint32_t DataHandler<T, L>::convert_to_little_endian(
    const unsigned char* bytes) {
  return (uint32_t)((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) |
                    (bytes[3]));
}

}  // namespace xuzy::ML::Data