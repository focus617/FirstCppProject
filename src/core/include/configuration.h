#pragma once

#include <iostream>

namespace xuzy
{
    class configuration
    {
    private:
        std::string m_configuration_filename;

        int open_file(const char *fileName);

    public:
        configuration(/* args */);
        ~configuration();
    };

    configuration::configuration(/* args */)
    {
    }

    configuration::~configuration()
    {
    }

    int configuration::open_file(const char *t_filename)
    {
        std::ifstream ifs(t_filename);

        if (!ifs.is_open())
        {
            throw std::exception("Failed to open configuration file.");
        }
        return 0;
    }

}