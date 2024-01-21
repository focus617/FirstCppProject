#pragma once

#include <httplib.h>

// Everything that is api related
namespace api::list {

void setup_routing(httplib::Server& svr);
void get_all(const httplib::Request& req, httplib::Response& res);
void get(const httplib::Request& req, httplib::Response& res);
void put(const httplib::Request& req, httplib::Response& res);
void post(const httplib::Request& req, httplib::Response& res);
void remove(const httplib::Request& req, httplib::Response& res);

}  // namespace api::list