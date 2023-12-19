#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/strings/str_join.h"

#include "log.h"
#include "xuzy_math.h"
#include "singleton.h"

using xuzy::Log;

// Define the flag 标志
// 参数: 1.标志类型 2.标志名 3.默认值 4.标志描述
ABSL_FLAG(bool, verbose, false, "Enable verbose mode");

int main(int argc, char *argv[])
{
    absl::SetProgramUsageMessage(
        absl::StrCat("This program does nothing.  Sample usage:\n",
                     argv[0], " <uselessarg1> <uselessarg2>"));
    // 标志解析
    auto undefined_flags = absl::ParseCommandLine(argc, argv);

    // 读取未解析的参数，按位置打印
    LOG_INFO("undefined flags: %s", absl::StrJoin(undefined_flags, " ").c_str());

    // 读取bool串类型标志
    LOG_INFO("flag verbose: %s", absl::GetFlag(FLAGS_verbose) ? "true" : "false");

    /*
        Log::SetLevel(Log::LevelTrace);

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

        Singleton *s1 = Singleton::get_instance(10);
        s1->print_unique_id();
        std::cout << "S1 memory location: " << s1 << std::endl;

        Singleton *s2 = Singleton::get_instance(5);
        s2->print_unique_id();
        std::cout << "S2 memory location: " << s2 << std::endl;
    */

    return EXIT_SUCCESS;
}