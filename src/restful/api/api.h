#pragma once

#include <httplib.h>
namespace http::api
{
    void list(const httplib::Request &req, httplib::Response &res);
    
} // namespace http::api