#pragma once

#include <string>
#include <json.hpp>

#include "argsparser.h"

using json = nlohmann::json;

namespace xuzy
{

    class App
    {
    private:
        std::string m_app_name;
        ArgsParser *p_cli_parser;

        static void init_logger(const char *app);

    protected:
        // Configuration
        virtual json load_configuration_from_file();
        virtual void setup(const json &configuration) = 0;

    public:
        App(std::string t_app_name);
        virtual ~App();
        virtual void run();
        
        // Command line parser
        void set_cli_parser(ArgsParser *p_parser);

        static void main(int argc, char *argv[], const std::string &version, App *app);

        void version_check(int argc, char *argv[], const std::string version);
        void dumpError(std::string error);
    };

} // namespace xuzy