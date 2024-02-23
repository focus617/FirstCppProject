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
   * @brief Static method for accessing singleton instance.
   * The first time this method is called, a Singleton object is constructed
   * and returned.  Consecutive calls will return the same object.
   * @return Singleton instance.
   */
  static Singleton* GetInstance() {
    // CodeGear C++Builder insists on a public destructor for the
    // default implementation.  Use this implementation to keep good OO
    // design with private destructor.
#if defined(__BORLANDC__)
    static Singleton* instance = new Singleton();  //  lazy inits once
    return *instance;
#else
    static Singleton instance;  // thread safe solution for init once
    return &instance;
#endif  // defined(__BORLANDC__)
  }

  ~Singleton() {
    LOG(INFO) << "singleton instance " << m_unique_name_ << " destroyed."
              << std::endl;
  };

  // We disallow copying Singleton.
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

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