#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace xuzy::Tools::BenchMark {

struct ProfileResult {
  std::string name;
  long long start, end;
  uint32_t thread_id;
};

struct InstrumentationSession {
  std::string name;
};

class XUZY_API Instrumentor {
 private:
  InstrumentationSession* m_current_session;
  std::ofstream m_output_stream;
  int m_profile_count;

 public:
  Instrumentor() : m_current_session(nullptr), m_profile_count(0) {}

  void begin_session(const std::string& name,
                     const std::string& filepath = "results.json") {
    m_output_stream.open(filepath);
    write_header();
    m_current_session = new InstrumentationSession{name};
  }

  void end_session() {
    write_footer();
    m_output_stream.close();
    delete m_current_session;
    m_current_session = nullptr;
    m_profile_count = 0;
  }

  void write_profile(const ProfileResult& result) {
    if (m_profile_count++ > 0) m_output_stream << ",";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_output_stream << "{";
    m_output_stream << "\"cat\":\"function\",";
    m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
    m_output_stream << "\"name\":\"" << name << "\",";
    m_output_stream << "\"ph\":\"X\",";
    m_output_stream << "\"pid\":0,";
    m_output_stream << "\"tid\":" << result.thread_id << ",";
    m_output_stream << "\"ts\":" << result.start;
    m_output_stream << "}";

    m_output_stream.flush();
  }

  void write_header() {
    m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
    m_output_stream.flush();
  }

  void write_footer() {
    m_output_stream << "]}";
    m_output_stream.flush();
  }

  static Instrumentor& get() {
    static Instrumentor instance;
    return instance;
  }
};

class XUZY_API InstrumentationTimer {
 public:
  InstrumentationTimer(const char* name) : m_name_(name), m_stopped_(false) {
    m_start_timepoint_ = std::chrono::high_resolution_clock::now();
  }

  ~InstrumentationTimer() {
    if (!m_stopped_) stop();
  }

  void stop() {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
                          m_start_timepoint_)
                          .time_since_epoch()
                          .count();
    long long end =
        std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint)
            .time_since_epoch()
            .count();

    uint32_t threadID =
        std::hash<std::thread::id>{}(std::this_thread::get_id());

    Instrumentor::get().write_profile({m_name_, start, end, threadID});

    m_stopped_ = true;
  }

 private:
  const char* m_name_;
  std::chrono::time_point<std::chrono::high_resolution_clock>
      m_start_timepoint_;
  bool m_stopped_;
};

}  // namespace xuzy::Tools::BenchMark

#define XUZY_PROFILE 1
#if XUZY_PROFILE

#  define XUZY_PROFILE_BEGIN_SESSION(name, filepath) \
    ::xuzy::Tools::BenchMark::Instrumentor::get().begin_session(name, filepath)

#  define XUZY_PROFILE_END_SESSION() \
    ::xuzy::Tools::BenchMark::Instrumentor::get().end_session()

#  define XUZY_PROFILE_SCOPE(name) \
    ::xuzy::Tools::BenchMark::InstrumentationTimer timer##__LINE__(name)

#  if defined(XUZY_OS_WINDOWS)
#    define XUZY_PROFILE_FUNCTION() XUZY_PROFILE_SCOPE(__FUNCSIG__)
#  elif defined(XUZY_OS_LINUX)
#    define XUZY_PROFILE_FUNCTION() XUZY_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#  endif

#else
#  define XUZY_PROFILE_BEGIN_SESSION(name, filepath)
#  define XUZY_PROFILE_END_SESSION()
#  define XUZY_PROFILE_SCOPE(name)
#  define XUZY_PROFILE_FUNCTION()
#endif