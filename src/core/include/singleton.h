#pragma once

#include <glog/logging.h>

#include <iostream>
#include <mutex>
class Singleton {
 private:
  static std::mutex m_mutex_;
  static int m_unique_id_;

 public:
  // call this to get access to the public functions
  static Singleton& Instance() {
    static Singleton* instance = new Singleton();  // inits once (lazy)
    return *instance;
  }

  Singleton() { LOG(INFO) << "creating a new singleton instance" << std::endl; }

  ~Singleton() { LOG(INFO) << "singleton instance destroyed" << std::endl; };

  void operator=(const Singleton&) = delete;

  int get_unique_id() { return this->m_unique_id_; }

  void set_unique_id(int id) {
    std::scoped_lock lk(m_mutex_);
    this->m_unique_id_ = id;
  }

  void print_unique_id() {
    LOG(INFO) << "Instance's unique id: " << this->m_unique_id_ << std::endl;
  }
};

int Singleton::m_unique_id_ = 0;
std::mutex Singleton::m_mutex_;