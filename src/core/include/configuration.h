#pragma once

#include <iostream>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

namespace xuzy
{
    class Configuration
    {
    private:
        std::string m_configuration_filename;

    public:
        Configuration(std::string filename);
        Configuration();
        virtual ~Configuration();

        json load_from_json_file();
    };

    Configuration::Configuration(std::string filename)
        : m_configuration_filename{filename}
    {
    }

    Configuration::Configuration()
        : m_configuration_filename{"configuration"}
    {
    }

    Configuration::~Configuration()
    {
    }

    json Configuration::load_from_json_file()
    {
        json json_config;

        try
        {
            std::ifstream config_file((m_configuration_filename + ".json").c_str());

            if (config_file.std::ios::eof())
            {
                throw std::runtime_error("Config file is empty");
            }
            // Make sure the config is open
            if (!config_file.is_open())
            {
                throw std::runtime_error("Can't open config");
            }
            config_file >> json_config;
        }
        catch (std::exception &e)
        {
            throw std::string(e.what());
        }
        catch (...)
        {
            throw "Can't open config";
        }
        return json_config;
    }

}