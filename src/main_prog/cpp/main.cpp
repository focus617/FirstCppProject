#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/strings/str_join.h"

#include "config.h"
#include "log.h"
#include "xuzy_math.h"
#include "singleton.h"

using xuzy::Log;

enum class LogLevel
{
    Critical = 0,
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

void set_log_level(const LogLevel &t_new_level);

void test_logger();
void test_singleton();
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
    parse_commandline(argc, argv);

    // thread_trial();
    thread_active_object();

    /*
        test_logger();
        test_singleton();
    */

    return EXIT_SUCCESS;
}

int parse_commandline(int argc, char *argv[])
{
    absl::SetProgramUsageMessage(
        absl::StrCat("Version ", PROJECT_VERSION, "\n",
                     "This program does nothing.\n\nSample usage:\n",
                     argv[0], " --verbose -F <configure_file> "));
    // Parsing flags during startup
    auto undefined_flags = absl::ParseCommandLine(argc, argv);

    // 读取自定义类型标志
    LogLevel level = absl::GetFlag(FLAGS_log);
    LOG_INFO("Get flag for log: %s", AbslUnparseFlag(level).c_str());
    set_log_level(level);

    // 读取未解析的参数，按位置打印
    LOG_DEBUG("Undefined flags: %s", absl::StrJoin(undefined_flags, " ").c_str());

    // 读取bool类型标志
    LOG_INFO("Get flag for verbose: %s", absl::GetFlag(FLAGS_verbose) ? "true" : "false");

    // 读取optional类型标志
    if (absl::GetFlag(FLAGS_F).has_value())
    {
        // flag was set on command line
        std::string config_file_name = absl::GetFlag(FLAGS_F).value();
        LOG_INFO("Get flag for config_file: %s", config_file_name.c_str());

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
    if (t_text == "critical")
    {
        *t_log_level = LogLevel::Critical;
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

// AbslUnparseFlag converts from an xuzy::Log::Level to a string.
// Returns a textual flag value corresponding to the xuzy::Log::Level `log_level`.
std::string AbslUnparseFlag(LogLevel t_log_level)
{
    switch (t_log_level)
    {
    case LogLevel::Critical:
        return "critical";
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
    Log::Level log_level;

    switch (t_new_level)
    {
    case LogLevel::Critical:
        log_level = Log::Level::Critical;
        break;
    case LogLevel::Error:
        log_level = Log::Level::Error;
        break;
    case LogLevel::Warn:
        log_level = Log::Level::Warn;
        break;
    case LogLevel::Info:
        log_level = Log::Level::Info;
        break;
    case LogLevel::Debug:
        log_level = Log::Level::Debug;
        break;
    case LogLevel::Trace:
        log_level = Log::Level::Trace;
        break;
    default:
        log_level = Log::Level::Trace;
    }
    Log::SetLevel(log_level);
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
    LOG_CRITICAL("%d: Test Log Level %s", i++, "Critical");

    int a = 12;
    int b = 56;
    LOG_INFO("%d + %d = %d", a, b, xuzy::add(a, b));
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