/**
 * @brief Atomic Log Stream Class for C++
 * URL: * http://www.inspirel.com/articles/Atomic_Log_Stream.html
 * Support stream-like API to relieve users from string formatting issues.
 * It's also thread-safe to allow deployment in the multi-threading environment.
 */
#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <version>

#include "visibility_control.hpp"

namespace xuzy {

/**
 * @brief Manipulation Flags
 * In addition to the STL string manipulation flags such as endl, left, hex
 * and setw, we support the following:
 *
 * type 1: consumed once, these flags are all cleared by an endl or
 * flush or streaming L_clearFlags.
 *  L_null         default
 *  L_clearFlags   clear all flag (as well as an endl or flush)
 *  L_concat       remove L_tabs flag,
 *  L_flush        insert an endl then flush buffer
 *  L_endl         insert '\n' then perform flush if L_allwaysFlush
 *  L_time         insert current date & time,
 *  L_location     insert file-name, function name etc,
 *
 * type 2: not consumed, given powers-of-two values
 *  L_tabs         insert tabs between fields,
 *  L_allwaysFlush flush on every endl
 */
enum Flags {
  L_null,
  L_clearFlags,
  L_concat,
  L_flush,
  L_endl,
  L_time,
  L_location,
  L_tabs = 8,
  L_allwaysFlush = 16
};

inline Flags operator+=(Flags& l_flag, Flags r_flag) {
  return l_flag = static_cast<Flags>(l_flag | r_flag);
}

inline Flags operator-=(Flags& l_flag, Flags r_flag) {
  return l_flag = static_cast<Flags>(l_flag & ~r_flag);
}

using Streamable = std::ostream;

/**
 * @brief This record object accumulates all data that is passed with the
 * generic insertion operator "<".
 *
 * Advantage of this solution is that logical record can be defined as the
 * longest sequence of chained calls to stream insertion operator, and record
 * boundariy naturally relies on C++ expression.
 * It's thread safe since each record uses separate and private buffer, nor
 * shared data for synchronization overhead.
 *
 * For example, log << a << b << c << " and " << x() << y() << z();
 */
class XUZY_API Stream_Formatter {
 public:
  XUZY_API
  Stream_Formatter(std::ostream& out = std::cout) : os_{out} {}

  XUZY_API
  virtual ~Stream_Formatter() {
    // lock the output stream
    std::scoped_lock<std::mutex> lk(mutex_);
    os_ << buffer_.str();
  }

  template <typename T>
  Stream_Formatter& operator<<(T const& value) {
    buffer_ << value;
    return *this;
  }

  // The manipulator streaming operators make use of decltype to
  // deduce the types for std::hex, dec & oct which are functions,
  // similarly for setw, setprecision and many other manipulators
  // that take an integer argument:
  Stream_Formatter& operator<<(decltype(std::hex) manip) {
    buffer_ << manip;
    return *this;
  }
  Stream_Formatter& operator<<(decltype(std::setw) manip) {
    buffer_ << manip;
    return *this;
  }

  // In this class, flush should be only called when end of C++ expression,
  // so std::endl is captured and replaced by the L_endl flag, which streams
  // a \n.
  Stream_Formatter& operator<<(
      decltype(std::endl<char, std::char_traits<char>>)) {
    return *this << L_endl;
  }

  /**
   * @brief Customized Flag handling
   */
  Stream_Formatter& operator<<(Flags flag) {
    switch (flag) {
      case L_time:
        logTime();
        break;
      case L_flush:
        flags_ = static_cast<Flags>(flags_ & L_allwaysFlush);
        buffer_ << "\n";
        flush();
        break;
      case L_clearFlags:
        flags_ = static_cast<Flags>(flags_ & L_allwaysFlush);
        break;
      case L_allwaysFlush:
        flags_ += L_allwaysFlush;
        break;
      case L_concat:
        removeFlag(L_tabs);
        break;
      case L_endl:
        buffer_ << "\n";
        if (flags_ & L_allwaysFlush) {
          flush();
        }
        [[fallthrough]];
      case L_null:
        break;
      default:
        addFlag(flag);
        break;
    }
    return *this;
  }

  Stream_Formatter& L_location(Stream_Formatter& out) {
#ifdef __cpp_lib_source_location
    auto srec = std::source_location::current();
    buffer_ << "[" << srec.line() << " " << srec.function_name() << "]";
#else

#endif
    return *this;
  }

 protected:
  virtual void flush() {
    os_ << buffer_.str();
    os_.flush();
  }

  virtual Stream_Formatter& logTime();

  bool is_tabs() const { return flags_ & L_tabs || has_time(); }
  bool is_null() const { return flags_ == L_null; }
  bool has_time() const { return (flags_ & 7) == L_time; }

 private:
  std::ostream& os_;           // final output stream
  std::ostringstream buffer_;  // private buffer for thread-safe
  std::mutex mutex_;
  Flags flags_ = L_null;

  Flags addFlag(Flags flag) { return flags_ += flag; }
  Flags removeFlag(Flags flag) { return flags_ -= flag; }

  static tm* getTime();

  struct Log_date {
    unsigned char dayNo;
    unsigned char monthNo;
  } inline static log_date{0, 0};
};

tm* Stream_Formatter::getTime() {
  std::time_t now = std::time(nullptr);
  auto localTime = std::localtime(&now);
  log_date.dayNo = localTime->tm_mday;
  log_date.monthNo = localTime->tm_mon + 1;
  return localTime;
}

Stream_Formatter& Stream_Formatter::logTime() {
  buffer_ << std::put_time(getTime(), "[%d/%m/%y %H:%M:%S] ");
  flags_ += L_time;
  return *this;
}

}  // namespace xuzy