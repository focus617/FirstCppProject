#pragma once

#include <httplib.h>
namespace http::api
{
    void lists_get(const httplib::Request &req, httplib::Response &res);
    void lists_post(const httplib::Request &req, httplib::Response &res);    
} // namespace http::api