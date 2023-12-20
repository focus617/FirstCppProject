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

enum LogLevel
{
    LevelCritical = 0,
    LevelError,
    LevelWarn,
    LevelInfo,
    LevelDebug,
    LevelTrace
};

void set_log_level(const LogLevel &level);

void test_logger();
void test_singleton();

// 从配置文件读取配置参数
void load_from_configure_file(const std::string &filename);

bool AbslParseFlag(absl::string_view text, LogLevel *log_level, std::string *error);
std::string AbslUnparseFlag(LogLevel log_level);

// Define the flag 标志
// 参数: 1.标志类型 2.标志名 3.默认值 4.标志描述
ABSL_FLAG(bool, verbose, false, "Enable verbose mode");
ABSL_FLAG(std::optional<std::string>, F, std::nullopt, "Settings from a file");
ABSL_FLAG(LogLevel, log, LogLevel::LevelInfo, "Setting log level");

int main(int argc, char *argv[])
{
    absl::SetProgramUsageMessage(
        absl::StrCat("This program does nothing.  Sample usage:\n",
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

    /*
        test_logger();
        test_singleton();
    */

    return EXIT_SUCCESS;
}

// Parses an OutputMode from the command line flag value `text`.
// Returns `true` and sets `*log_level` on success;
// Returns `false` and sets `*error` on failure.
bool AbslParseFlag(absl::string_view text,
                   LogLevel *log_level,
                   std::string *error)
{
    if (text == "critical")
    {
        *log_level = LogLevel::LevelCritical;
        return true;
    }
    if (text == "error")
    {
        *log_level = LogLevel::LevelError;
        return true;
    }
    if (text == "warn")
    {
        *log_level = LogLevel::LevelWarn;
        return true;
    }
    if (text == "info")
    {
        *log_level = LogLevel::LevelInfo;
        return true;
    }
    if (text == "debug")
    {
        *log_level = LogLevel::LevelDebug;
        return true;
    }
    if (text == "trace")
    {
        *log_level = LogLevel::LevelTrace;
        return true;
    }
    *error = "unknown value for log level";
    return false;
}

// AbslUnparseFlag converts from an xuzy::Log::Level to a string.
// Returns a textual flag value corresponding to the xuzy::Log::Level `log_level`.
std::string AbslUnparseFlag(LogLevel log_level)
{
    switch (log_level)
    {
    case LogLevel::LevelCritical:
        return "critical";
    case LogLevel::LevelError:
        return "error";
    case LogLevel::LevelWarn:
        return "warn";
    case LogLevel::LevelInfo:
        return "info";
    case LogLevel::LevelDebug:
        return "debug";
    case LogLevel::LevelTrace:
        return "trace";
    default:
        return std::to_string(static_cast<int>(log_level));
    }
}

void load_from_configure_file(const std::string &filename)
{
    LOG_INFO("%s(): Load configure from file: %s", __FUNCTION__, filename.c_str());
}

void set_log_level(const LogLevel &new_level)
{
    Log::Level log_level;

    switch (new_level)
    {
    case LogLevel::LevelCritical:
        log_level = Log::LevelCritical;
        break;
    case LogLevel::LevelError:
        log_level = Log::LevelError;
        break;
    case LogLevel::LevelWarn:
        log_level = Log::LevelWarn;
        break;
    case LogLevel::LevelInfo:
        log_level = Log::LevelInfo;
        break;
    case LogLevel::LevelDebug:
        log_level = Log::LevelDebug;
        break;
    case LogLevel::LevelTrace:
        log_level = Log::LevelTrace;
        break;
    default:
        log_level = Log::LevelTrace;
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
    int result = xuzy::add(a, b);
    LOG_INFO("%d + %d = %d", a, b, result);
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