#include <glog/logging.h>

#include <cmath>
#include <limits>
#include <map>

#include "data/data_handler.hpp"
#include "stdint.h"

namespace xuzy::ML::CLASSIFIER {

template <typename T, typename L>
KnnClassifier<T, L>::KnnClassifier(int k) : k_{k} {
  neighbors_ = new std::vector<Data::Data<T, L>*>;
}

template <typename T, typename L>
KnnClassifier<T, L>::KnnClassifier() {
  neighbors_ = new std::vector<Data::Data<T, L>*>;
}

template <typename T, typename L>
KnnClassifier<T, L>::~KnnClassifier() {
  if (nullptr != neighbors_) delete neighbors_;
}

template <typename T, typename L>
void KnnClassifier<T, L>::set_k(int p_value) {
  k_ = p_value;
}

template <typename T, typename L>
double KnnClassifier<T, L>::validate_performance() {
  return performance(this->validation_data_, true);
}

template <typename T, typename L>
double KnnClassifier<T, L>::test_performance() {
  return performance(this->test_data_, false);
}

template <typename T, typename L>
double KnnClassifier<T, L>::performance(
    std::vector<Data::Data<T, L>*>* p_data_set, bool show_detail) {
  int correct_count = 0;
  int data_index = 0;

  for (Data::Data<T, L>* query_point : *p_data_set) {
    find_knearest(query_point);
    if (predict() == query_point->get_label()) {
      ++correct_count;
    }
    ++data_index;
    if (show_detail) {
      LOG(INFO) << data_index << ": "
                << "Current Performance = "
                << ((double)correct_count * 100) / ((double)data_index) << " %";
    }
  }
  return ((double)correct_count * 100) / ((double)p_data_set->size());
}

// if K ~ N : O(N^2)
// if K = 2 : O(~N)
// else:      O(NlogN)
template <typename T, typename L>
void KnnClassifier<T, L>::find_knearest(Data::Data<T, L>* p_query_point) {
  double min = std::numeric_limits<double>::max();
  double previous_min = min;

  neighbors_->clear();

  int index = 0;
  for (int i = 0; i < k_; i++) {
    if (i == 0) {
      for (unsigned j = 0; j < this->training_data_->size(); j++) {
        double distance =
            calculate_distance(p_query_point, this->training_data_->at(j));
        this->training_data_->at(j)->set_distance(distance);
        if (distance < min) {
          min = distance;
          index = j;
        }
      }
    } else {
      for (unsigned j = 0; j < this->training_data_->size(); j++) {
        double distance = this->training_data_->at(j)->get_distance();
        if (distance > previous_min && distance < min) {
          min = distance;
          index = j;
        }
      }
    }
    neighbors_->push_back(this->training_data_->at(index));
    previous_min = min;
    min = std::numeric_limits<double>::max();
  }
}

template <typename T, typename L>
double KnnClassifier<T, L>::calculate_distance(Data::Data<T, L>* p_query_point,
                                               Data::Data<T, L>* p_input) {
  if (p_query_point->get_feature_vector_size() !=
      p_input->get_feature_vector_size()) {
    LOG(WARNING) << "Vector Size Mismatch!";
    exit(1);
  }

  double distance = 0.0;

#if defined(EUCLIDEAN)
  for (int i = 0; i < p_query_point->get_feature_vector_size(); i++) {
    distance += pow(p_query_point->get_feature_vector()->at(i) -
                        p_input->get_feature_vector()->at(i),
                    2);
  }
  distance = sqrt(distance);

#elif defined(MANHATTAN)
// Put MANHATTAN Implementation here
#endif
  return distance;
}

template <typename T, typename L>
L KnnClassifier<T, L>::predict() {
  std::map<L, int> class_freq;

  for (unsigned i = 0; i < neighbors_->size(); i++) {
    L label = neighbors_->at(i)->get_label();
    if (class_freq.find(label) == class_freq.end()) {
      class_freq[label] = 1;
    } else {
      class_freq[label]++;
    }
  }

  L best;
  int max = 0;
  for (auto kv : class_freq) {
    if (kv.second > max) {
      max = kv.second;
      best = kv.first;
    }
  }
  return best;
}

}  // namespace xuzy::ML::CLASSIFIER