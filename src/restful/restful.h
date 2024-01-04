#pragma once

#include <string>

namespace xuzy
{
    class Restful
    {
    private:
        std::string m_app_name;

    public:
        Restful(std::string appName);
        virtual ~Restful() final = default;

        Restful(const Restful &) = delete;
        Restful &operator=(const Restful &) = delete;
        Restful(Restful &&) = delete;
        Restful &operator=(Restful &&) = delete;

        void run();
    };
}