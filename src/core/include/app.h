#pragma once

#include <string>
#include <json.hpp>
using json = nlohmann::json;

namespace xuzy
{

    class App
    {
    private:
        std::string m_app_name;

    protected:
        // Configuration
        virtual json load_configuration_from_file();
        virtual void setup(const json &configuration) = 0;

    public:
        App(std::string t_app_name);
        virtual ~App();
        virtual void run();

        static void main(int argc, char *argv[], const std::string &version, App *app);

        void version_check(int argc, char *argv[], const std::string version);
        void dumpError(std::string error);
    };

} // namespace xuzy