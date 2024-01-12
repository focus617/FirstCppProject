#include <string>

#include "app/argsparser.h"

namespace xuzy
{
    // Reads the environment variable with name `name` and stores results in
    // `value`. If variable is not present in environment returns false, 
    // otherwise returns true.
    bool ArgsParser::GetEnvVar(const char *var_name, std::string &var_value)
    {
#ifdef _WIN32
        char buf[1024];
        auto get_res = GetEnvironmentVariableA(var_name, buf, sizeof(buf));
        if (get_res >= sizeof(buf))
        {
            return false;
        }
        if (get_res == 0)
        {
            return false;
        }
        var_value = std::string(buf, get_res);
#else
        const char *val = ::getenv(var_name);
        if (val == nullptr)
        {
            return false;
        }
        var_value = val;
#endif
        return true;
    }

} // namespace xuzy