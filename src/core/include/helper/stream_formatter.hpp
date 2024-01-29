/**
 * @brief Atomic Log Stream Class for C++
 * URL: * http://www.inspirel.com/articles/Atomic_Log_Stream.html
 * Support stream-like API to relieve users from string formatting issues.
 * It's also thread-safe to allow deployment in the multi-threading environment.
 *
 * Typical usage:
 *
 *   1. You stream a bunch of values to a Stream_Formatter object.
 *      It will remember the text in a stringstream.
 *   2. When the Stream_Formatter object out-of-scope, this causes
 *      the text in the Stream_Formatter to be streamed to the ostream.
 *   3. The ostream is configurable with std:cout by default.
 *
 * For example;
 *  {
 *    xuzy::Stream_Formatter foo;
 *    foo << 1 << " != " << 2;
 *  }
 *
 * will print "1 != 2".
 */
#pragma once

#include <chrono>
#include <ctime>
// #include <format>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <version>

#include "helper/char_encoding.h"
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
 *  L_title        insert title at begin of each record/line
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
  L_allwaysFlush = 16,
  L_title = 32
};

inline Flags operator+=(Flags& l_flag, Flags r_flag) {
  return l_flag = static_cast<Flags>(l_flag | r_flag);
}

inline Flags operator-=(Flags& l_flag, Flags r_flag) {
  return l_flag = static_cast<Flags>(l_flag & ~r_flag);
}

/**
 * @brief This record object accumulates all data that is passed with the
 * generic insertion operator "<<".
 *
 * Advantage of this solution is that logical record can be defined as the
 * longest sequence of chained calls to stream insertion operator, and record
 * boundariy naturally relies on C++ expression.
 * It's thread safe since each record uses separate and private buffer, nor
 * shared data for synchronization overhead.
 */
class XUZY_API Stream_Formatter {
  using Streamable = std::ostream;

 public:
  XUZY_API
  Stream_Formatter(Streamable& out = std::cout)
      : os_{out}, timestamp_format_{"%F %T"} {}

  XUZY_API
  virtual ~Stream_Formatter() { flush(); }

  /**
   * @brief Streams a non-pointer value to this object.
   */
  template <typename T>
  inline Stream_Formatter& operator<<(T const& value) {
    if (is_tabs()) buffer_ << "\t";
    buffer_ << value;
    return *this;
  }

  /**
   * @brief Streams a pointer value to this object.
   *
   * This function is an overload of the previous one.  When you stream
   * a pointer to a Message, this definition will be used as it is more
   * specialized.  (The C++ Standard, section [temp.func.order].)
   * If you stream a non-pointer, then the previous definition will be used.
   *
   * The reason for this overload is that streaming a NULL pointer to
   * ostream is undefined behavior.  Depending on the compiler, you may
   * get "0", "(nil)", "(null)", or an access violation.
   * To ensure consistent result across compilers, we always treat NULL
   * as "(null)".
   */
  template <typename T>
  inline Stream_Formatter& operator<<(T* const& pointer) {  // NOLINT
    if (pointer == nullptr) {
      buffer_ << "(null)";
    } else {
      buffer_ << pointer;
    }
    return *this;
  }

  // Instead of 1/0, we want to see true/false for bool values.
  inline Stream_Formatter& operator<<(bool b) {
    buffer_ << (b ? "true" : "false");
    return *this;
  }

  // when streams char, converting NUL bytes to "\\0" along the way.
  inline Stream_Formatter& operator<<(char ch) {
    if (ch == '\0') {
      buffer_ << "\\0";  // Replaces NUL with "\\0";
    } else {
      buffer_ << ch;
    }
    return *this;
  }

  // These two overloads allow streaming a wide C string to a Message
  // using the UTF-8 encoding.
  Stream_Formatter& operator<<(const wchar_t* wide_c_str) {
    return *this << xuzy::internal::ShowWideCString(wide_c_str);
  }

  // Stream_Formatter& operator<<(wchar_t* wide_c_str) {
  //    return *this << xuzy::internal::ShowWideCString(wide_c_str);
  // }

  Stream_Formatter& operator<<(wchar_t* wide_c_str);

  // when streams std::string, converting NUL bytes to "\\0" along the way.
  inline Stream_Formatter& operator<<(std::string str) {
    const char* const start = str.c_str();
    const char* const end = start + str.length();

    std::string result;
    result.reserve(static_cast<size_t>(2 * (end - start)));
    for (const char* ch = start; ch != end; ++ch) {
      if (*ch == '\0') {
        result += "\\0";  // Replaces NUL with "\\0";
      } else {
        result += *ch;
      }
    }

    buffer_ << result;
    return *this;
  }

  // Since the basic IO manipulators are overloaded for both narrow
  // and wide streams, we have to provide this specialized definition
  // of operator <<, even though its body is the same as the
  // templatized version above.  Without this definition, streaming
  // endl or other basic IO manipulators to Stream_Formatter will confuse
  // the compiler.
  Stream_Formatter& operator<<(Stream_Formatter& (*manip)(Stream_Formatter&)) {
    (*manip)(*this);
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
  // or L_allwaysFlush flag is set, so std::endl is captured and replaced by
  // the L_endl flag, which streams a '\n' then decide wether flush.
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
        put_timestamp();
        break;
      case L_flush:
        flags_ = static_cast<Flags>(flags_ & (L_allwaysFlush | L_title));
        buffer_ << "\n";
        flush();
        break;
      case L_clearFlags:
        clearFlag();
        buffer_ << "\n";
        flush();
        break;
      case L_concat:
        removeFlag(L_tabs);
        break;
      case L_location:
        // #ifdef __cpp_lib_source_location
        //         auto srec = std::source_location::current();
        //         buffer_ << "[" << srec.line() << " " <<
        //         srec.function_name()
        //         << "]";
        // #else
        buffer_ << "[" << __LINE__ << " " << __FUNCTION__ << "]";
        // #endif
        break;
      case L_endl:
        buffer_ << "\n";
        if (flags_ & L_allwaysFlush) {
          flush();
        }
        [[fallthrough]];
      case L_null:
        break;
      case L_allwaysFlush:
        flags_ += L_allwaysFlush;
        break;
      default:
        addFlag(flag);
        break;
    }
    return *this;
  }

  /**
   * @brief Get the text streamed to this object so far as an std::string.
  // Note: Each '\0' character in the buffer was replaced with "\\0".
  //
  // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
  */
  XUZY_LOCAL
  const std::string get_string() { return buffer_.str(); }

  /**
   * @brief Set format for timestamp
   * @param format Pointer to a null-terminated Char string specifying the
   * format of conversion
   */
  XUZY_API
  virtual void set_timestamp_format(const char* format) {
    timestamp_format_ = format;
  }

  XUZY_LOCAL bool is_null() const { return flags_ == L_null; }
  XUZY_LOCAL bool is_tabs() const { return flags_ & L_tabs; }
  XUZY_LOCAL bool is_alwaysflush() const { return flags_ & L_allwaysFlush; }
  XUZY_LOCAL bool has_time() const { return (flags_ & 7) == L_time; }

 protected:
  virtual void flush() {
    // lock the output stream
    std::scoped_lock<std::mutex> lk(mutex_);
    os_ << buffer_.str();
    buffer_.str("");  // 清空缓冲区
    buffer_.clear();  // 重置流状态
    os_.flush();
  }

  virtual void put_timestamp() {
    buffer_ << "[" << std::put_time(get_localtime(), timestamp_format_.c_str())
            << "]";
  }

 private:
  Streamable& os_;             // final output stream
  std::ostringstream buffer_;  // private buffer for thread-safe
  std::mutex mutex_;

  Flags flags_ = L_null;
  std::string timestamp_format_;  // output format for timestamp

  Flags addFlag(Flags flag) { return flags_ += flag; }
  Flags removeFlag(Flags flag) { return flags_ -= flag; }
  void clearFlag() { flags_ = L_null; }

  struct Log_date {
    unsigned char day;
    unsigned char month;
  } inline static log_date{1, 1};

  std::tm* get_localtime() {
    std::time_t now = std::time({});
    return std::localtime(&now);
  }

  void record_time() {
    auto localTime = get_localtime();
    log_date.day = localTime->tm_mday;
    log_date.month = localTime->tm_mon + 1;
  }
};

// Streams a Stream_Formatter to an ostream.
inline std::ostream& operator<<(std::ostream& os, Stream_Formatter& sf) {
  return os << sf.get_string();
}

/**
 * @brief Converts a streamable value to an std::string.
 *
 * - A NULL pointer is converted to "(null)".
 * - When the input value is a ::string, ::std::string, ::wstring, or
 * ::std::wstring object, each NUL character in it is replaced with "\\0".
 */
template <typename T>
std::string StreamableToString(const T& streamable) {
  return (Stream_Formatter() << streamable).get_string();
}

}  // namespace xuzy