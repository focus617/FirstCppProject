#include <string>

#include "log.h"
#include "xuzy_math.h"
#include "singleton.h"

using xuzy::Log;

int main(){

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

    Singleton* s1 = Singleton::get_instance(10);
    s1->print_unique_id();
    std::cout << "S1 memory location: " << s1 << std::endl;

    Singleton* s2 = Singleton::get_instance(5);
    s2->print_unique_id();
    std::cout << "S2 memory location: " << s2 << std::endl;

    return EXIT_SUCCESS;
}