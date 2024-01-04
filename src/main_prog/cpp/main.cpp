#include <string>
#include <sstream>
#include <thread>

#include <glog/logging.h>
#include <glog/stl_logging.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/strings/str_join.h"

#include "config.h"
#include "logger.h"
#include "xuzy_math.h"
#include "singleton.h"
#include "restful.h"

using xuzy::Logger;

enum LogLevel
{
    Fatal = 0,
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

void set_log_level(const LogLevel &t_new_level);

void test_logger();
void test_singleton();
extern int test_httplib_client();
extern void thread_trial();
extern void thread_active_object();

// 从配置文件读取配置参数
void load_from_configure_file(const std::string &t_filename);

bool AbslParseFlag(absl::string_view t_text, LogLevel *t_log_level, std::string *t_error);
std::string AbslUnparseFlag(LogLevel t_log_level);

int parse_commandline(int argc, char *argv[]);

// Define the flag 标志
// 参数: 1.标志类型 2.标志名 3.默认值 4.标志描述
ABSL_FLAG(bool, verbose, false, "Enable verbose mode");
ABSL_FLAG(std::optional<std::string>, F, std::nullopt, "Settings from a file");
ABSL_FLAG(LogLevel, log, LogLevel::Info, "Setting log level");

int main(int argc, char *argv[])
{
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    // Log both to log file and stderr
    FLAGS_alsologtostderr = true;

    parse_commandline(argc, argv);

    http::Host host("0.0.0.0", 8080);
    std::thread(&http::listener::run, std::move(host)).detach();

    std::cin.get();
    LOG(INFO) << "Main Thread Stopped.";

    /*
        test_logger();
        test_singleton();

        thread_trial();
        thread_active_object();
    */

    return EXIT_SUCCESS;
}

int parse_commandline(int argc, char *argv[])
{
    absl::SetProgramUsageMessage(
        absl::StrCat("Version ", PROJECT_VERSION, "\n",
                     "This program does nothing.\n\nSample usage:\n",
                     argv[0], " --verbose -F <configure_file> --log==[fatal|error|warn|info|debug|trace]"));

    // Parsing flags during startup
    auto undefined_flags = absl::ParseCommandLine(argc, argv);

    // 读取自定义类型标志
    LogLevel level = absl::GetFlag(FLAGS_log);
    LOG(INFO) << "Get flag for log: " << AbslUnparseFlag(level);
    set_log_level(level);

    // 读取未解析的参数，按位置打印
    LOG_DEBUG("Undefined flags: %s", absl::StrJoin(undefined_flags, " ").c_str());

    // 读取bool类型标志
    LOG(INFO) << "Get flag for verbose: " << (absl::GetFlag(FLAGS_verbose) ? "true" : "false");

    // 读取optional类型标志
    if (absl::GetFlag(FLAGS_F).has_value())
    {
        // flag was set on command line
        std::string config_file_name = absl::GetFlag(FLAGS_F).value();
        LOG(INFO) << "Get flag for config_file: " << config_file_name;

        load_from_configure_file(config_file_name);
    }
    else
    {
        // flag was not passed on command line
        // use default configuration
    }

    return EXIT_SUCCESS;
}

// Parses an OutputMode from the command line flag value `text`.
// Returns `true` and sets `*log_level` on success;
// Returns `false` and sets `*error` on failure.
bool AbslParseFlag(absl::string_view t_text,
                   LogLevel *t_log_level,
                   std::string *t_error)
{
    if (t_text == "fatal")
    {
        *t_log_level = LogLevel::Fatal;
        return true;
    }
    if (t_text == "error")
    {
        *t_log_level = LogLevel::Error;
        return true;
    }
    if (t_text == "warn")
    {
        *t_log_level = LogLevel::Warn;
        return true;
    }
    if (t_text == "info")
    {
        *t_log_level = LogLevel::Info;
        return true;
    }
    if (t_text == "debug")
    {
        *t_log_level = LogLevel::Debug;
        return true;
    }
    if (t_text == "trace")
    {
        *t_log_level = LogLevel::Trace;
        return true;
    }
    *t_error = "unknown value for log level";
    return false;
}

// AbslUnparseFlag converts from an xuzy::Logger::Level to a string.
// Returns a textual flag value corresponding to the xuzy::Logger::Level `log_level`.
std::string AbslUnparseFlag(LogLevel t_log_level)
{
    switch (t_log_level)
    {
    case LogLevel::Fatal:
        return "fatal";
    case LogLevel::Error:
        return "error";
    case LogLevel::Warn:
        return "warn";
    case LogLevel::Info:
        return "info";
    case LogLevel::Debug:
        return "debug";
    case LogLevel::Trace:
        return "trace";
    default:
        return std::to_string(static_cast<int>(t_log_level));
    }
}

void load_from_configure_file(const std::string &t_filename)
{
    LOG_INFO("%s(): Load configure from file: %s", __FUNCTION__, t_filename.c_str());
}

void set_log_level(const LogLevel &t_new_level)
{
    Logger::Level log_level;

    switch (t_new_level)
    {
    case LogLevel::Fatal:
        log_level = Logger::Level::Fatal;
        break;
    case LogLevel::Error:
        log_level = Logger::Level::Error;
        break;
    case LogLevel::Warn:
        log_level = Logger::Level::Warn;
        break;
    case LogLevel::Info:
        log_level = Logger::Level::Info;
        break;
    case LogLevel::Debug:
        log_level = Logger::Level::Debug;
        break;
    case LogLevel::Trace:
        log_level = Logger::Level::Trace;
        break;
    default:
        log_level = Logger::Level::Trace;
    }
    Logger::get_logger().set_level(log_level);
    return;
}

void test_logger()
{
    int i{0};
    LOG_TRACE("%d: Test Log Level %s", i++, "Trace");
    LOG_DEBUG("%d: Test Log Level %s", i++, "Debug");
    LOG_INFO("%d: Test Log Level %s", i++, "Info");
    LOG_WARN("%d: Test Log Level %s", i++, "Warn");
    LOG_ERROR("%d: Test Log Level %s", i++, "Error");
    LOG_FATAL("%d: Test Log Level %s", i++, "Fatal");

    auto message1 = std::stringstream();
    message1 << i << ": Test Log Level with new logger.";

    int a = 12;
    int b = 56;
    auto message2 = std::stringstream();
    message2 << a << " + " << b << " = " << xuzy::add(a, b);

    // Getting the logger instance.
    auto &logger = Logger::get_logger();
    Logger::set_output(&std::cout);

    logger.FATAL(message1.str(), __LINE__, __FILE__);
    logger.ERROR(message1.str(), __LINE__, __FILE__);
    logger.WARN(message1.str(), __LINE__, __FILE__);
    logger.INFO(message2.str());
    logger.DEBUG(message1.str(), __LINE__, __FILE__);
    logger.TRACE(message1.str(), __LINE__, __FILE__);
}

void test_singleton()
{
    Singleton *s1 = Singleton::get_instance(10);
    s1->print_unique_id();
    std::cout << "S1 memory location: " << s1 << std::endl;

    Singleton *s2 = Singleton::get_instance(5);
    s2->print_unique_id();
    std::cout << "S2 memory location: " << s2 << std::endl;
}