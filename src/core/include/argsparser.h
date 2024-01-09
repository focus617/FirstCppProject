#pragma once

namespace xuzy
{
    class ArgsParser
    {
    private:
        bool GetEnvVar(const char *var_name, std::string &var_value);

    public:
        ArgsParser() = default;
        virtual ~ArgsParser() = default;

        virtual bool parse_commandline(int argc, char *argv[]) = 0;
    };

} // namespace xuzy