#include <iostream>
#include "xuzy_math.h"
#include "log.h"

int xuzy::add(int a, int b)
{
    LOG_INFO("%s", xuzy::return_by_macro());
    return a + b;
}

const char *xuzy::return_by_macro()
{
#ifdef USE_MYMATH
    return "USE_MYMATH is defined";
#else
    return "USE_MYMATH is not defined";
#endif
}