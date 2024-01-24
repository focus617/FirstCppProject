#pragma once

#include <glog/logging.h>

#include <iostream>
#include <mutex>

/**
 * @brief Singleton Base Class
 */
class Singleton {
 private:
  static std::mutex m_mutex_;
  static std::string m_unique_name_;

 public:
  /**
   * @brief Static method for accessing class instance.
   * Part of Singleton design pattern.
   *
   * @return Singleton instance.
   */
  static Singleton& Instance() {
    // static Singleton* instance = new Singleton();  //  lazy inits once
    // return *instance;
    static Singleton instance;      // thread safe solution for init once
    return instance;
  }

  ~Singleton() {
    LOG(INFO) << "singleton instance " << m_unique_name_ << " destroyed."
              << std::endl;
  };
  void operator=(const Singleton&) = delete;

  static std::string get_unique_name() { return Singleton::m_unique_name_; }

  static void set_unique_name(std::string name) {
    std::scoped_lock lk(m_mutex_);
    Singleton::m_unique_name_ = name;
  }

  static void print_unique_name() {
    LOG(INFO) << "Instance's unique name: " << Singleton::m_unique_name_
              << std::endl;
  }

 private:
  Singleton() { LOG(INFO) << "creating a new singleton instance" << std::endl; }
};

std::string Singleton::m_unique_name_;
std::mutex Singleton::m_mutex_;