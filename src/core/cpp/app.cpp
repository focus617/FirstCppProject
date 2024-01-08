
#include <iostream>
#include <fstream>
#include <glog/logging.h>

#include "app.h"

using namespace xuzy;

App::App(std::string t_app_name) : m_app_name{t_app_name}
{
}

App::~App()
{
}

void App::run()
{
    // Setup configuration
	json configuration = load_configuration_from_file();

    // Call the subclass with the configuration
    setup(configuration);
}

void App::version_check(int argc, char *argv[], const std::string version)
{
    LOG(INFO) << m_app_name << " version: " << version << std::endl;
}

void App::dumpError(std::string error)
{
    LOG(INFO) << m_app_name << " error: " << error << std::endl;
}

json App::load_configuration_from_file()
{
    json configuration;
    try
    {
        std::ifstream config_file((m_app_name + ".json").c_str());

        if (config_file.std::ios::eof())
        {
            throw std::runtime_error("Config file is empty");
        }
        // Make sure the config is open
        if (!config_file.is_open())
        {
            throw std::runtime_error("Can't open config");
        }
        config_file >> configuration;
    }
    catch (std::exception &e)
    {
        throw std::string(e.what());
    }
    catch (...)
    {
        throw "Can't open config";
    }
    return configuration;
}

void App::main(int argc, char *argv[], const std::string &version, App *app = nullptr)
{
    // Initialize Google’s logging library.
    // google::InitGoogleLogging(argv[0]);
    // Log both to log file and stderr
    // FLAGS_alsologtostderr = true;

    if (app)
    {
        // Show outself, then run
        try
        {
            app->version_check(argc, argv, version);

            // Run forever
            app->run();
        }
        catch (std::string s)
        {
            app->dumpError(s);
        }
        catch (const char *s)
        {
            app->dumpError(s);
        }
        catch (std::exception &e)
        {
            app->dumpError(e.what());
        }
        catch (...)
        {
            app->dumpError("Unknown");
        }
    }

    // Cleanup
    if (app)
    {
        delete app;
    }
}