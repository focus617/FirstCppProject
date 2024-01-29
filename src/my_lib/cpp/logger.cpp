#include "logger.h"

#include <iostream>
#include <sstream>

#include "helper/timer.hpp"

using namespace xuzy;

Logger* Logger::m_one_and_only_instance = nullptr;

// Initialization of the display pointer.
std::ostream* Logger::p_output_ = nullptr;

// Initialization of the display protection mutex.
std::mutex Logger::m_mutex_output;

Logger::Level Logger::m_level{Logger::Level::Info};

Logger& Logger::get_logger() {
  // The logger variable is initialized once with a move assignment of a
  // logger object build with the private constructor.
  static auto&& logger = Logger();
  return (logger);
}

void Logger::set_level(const Level t_level) {
  LOG_INFO("Set log level to: %s", to_string(m_level));
  {
    std::scoped_lock locker(m_mutex_output);
    m_level = t_level;
  }
}

void Logger::set_output(std::ostream* t_output) {
  std::scoped_lock locker(m_mutex_output);
  p_output_ = t_output;
}

void Logger::set_log_location(const std::string& t_app_name,
                              const std::string& t_location) {
  std::ostringstream log_filename;
  log_filename << t_location << "/" << t_app_name << ".log";

  this->m_log_filename = log_filename.str();
}

void Logger::print(enum Level t_message_level, const std::string& t_message) {
  if (m_level >= t_message_level) {
    // Initializing a string stream.
    auto ss = std::stringstream();

    // Buffering the timestamp in the string stream.
    ss << xuzy::Timer::get_current_timestamp() << "\t";

    // Buffering the log level in the string stream.
    ss << "[" << to_string(t_message_level) << "]:";

    // Buffering the message in the string stream.
    ss << t_message << '\n';

    // Waiting for output access.
    {
      std::scoped_lock locker(m_mutex_output);

      if (p_output_ != nullptr) {
        // If the output is set, then display the string stream.
        (*p_output_) << ss.rdbuf() << std::endl;
      }
    }

    // TODO: save log into file
  }
}

void Logger::print(int t_line_number, const char* t_source_file_name,
                   enum Level t_message_level, const std::string& t_message) {
  if (m_level >= t_message_level) {
    // Initializing a string stream.
    auto ss = std::stringstream();

    // Buffering the timestamp in the string stream.
    ss << xuzy::Timer::get_current_timestamp() << "\t";

    // Buffering the log level in the string stream.
    ss << "[" << to_string(t_message_level) << "]:";

    // Buffering the message in the string stream.
    ss << t_message;

    // Buffering the line and filename in the string stream.
    ss << "\n\t\t\t\t"
       << "on line " << t_line_number << " in " << t_source_file_name << '\n';

    // Waiting for output access.
    {
      std::scoped_lock locker(m_mutex_output);

      if (p_output_ != nullptr) {
        // If the output is set, then display the string stream.
        (*p_output_) << ss.rdbuf() << std::endl;
      }
    }

    // TODO: save log into file
  }
}

const char* Logger::to_string(const Level& t_log_level) {
  switch (t_log_level) {
    case Logger::Level::Fatal:
      return "FATAL";
    case Logger::Level::Error:
      return "ERROR";
    case Logger::Level::Warn:
      return "WARN";
    case Logger::Level::Info:
      return "INFO";
    case Logger::Level::Debug:
      return "DEBUG";
    case Logger::Level::Trace:
      return "TRACE";
    default:
      return "error log level";
  }
}

void Logger::FATAL(const std::string& t_message, int t_line_number,
                   const char* t_source_file_name) {
  print(t_line_number, t_source_file_name, Level::Fatal, t_message);
}

void Logger::ERROR(const std::string& t_message, int t_line_number,
                   const char* t_source_file_name) {
  print(t_line_number, t_source_file_name, Level::Error, t_message);
}

void Logger::WARN(const std::string& t_message, int t_line_number,
                  const char* t_source_file_name) {
  print(t_line_number, t_source_file_name, Level::Warn, t_message);
}

void Logger::INFO(const std::string& t_message) {
  print(Level::Info, t_message);
}

void Logger::DEBUG(const std::string& t_message, int t_line_number,
                   const char* t_source_file_name) {
  print(t_line_number, t_source_file_name, Level::Debug, t_message);
}

void Logger::TRACE(const std::string& t_message, int t_line_number,
                   const char* t_source_file_name) {
  print(t_line_number, t_source_file_name, Level::Trace, t_message);
}

/////////////////////////////////////////////////

void Logger::print_timestamp() {
  std::time_t now = std::time(0);
  std::tm* localtime = std::localtime(&now);

  char buffer[80];
  strftime(buffer, 80, "%c", localtime);
  printf("%s", buffer);
}
