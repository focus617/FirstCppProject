#pragma once

#include <httplib.h>


namespace api::list {
void setup_routing(httplib::Server& svr);
void get(const httplib::Request& req, httplib::Response& res);
void put(const httplib::Request& req, httplib::Response& res);
void post(const httplib::Request& req, httplib::Response& res);
void remove(const httplib::Request& req, httplib::Response& res);

}  // namespace api::list