#include "api.h"

#include <glog/logging.h>

#include <nlohmann/json.hpp>

#include "http/host.hpp"
#include "http/response_codes.h"

using json = nlohmann::json;

namespace api::list {

void setup_routing(httplib::Server& svr) {
  // Match the request path against a regular expression and extract its
  // captures
  svr.Get(R"(/api/v1/lists)",
          [&](const httplib::Request& req, httplib::Response& res) -> void {
            api::list::get_all(req, res);
          })

      .Get(R"(/api/v1/lists/(\d+))",
           [&](const httplib::Request& req, httplib::Response& res) -> void {
             api::list::get(req, res);
           })

      .Put(R"(/api/v1/lists/(\d+))",
           [&](const httplib::Request& req, httplib::Response& res) {
             api::list::put(req, res);
           })

      .Post(R"(/api/v1/lists)",
            [&](const httplib::Request& req, httplib::Response& res) {
              api::list::post(req, res);
            })

      .Delete(R"(/api/v1/lists/(\d+))",
              [&](const httplib::Request& req, httplib::Response& res) {
                api::list::remove(req, res);
              });
}

void get_all(const httplib::Request& req, httplib::Response& res) {
  res.status = http::Code::OK;
  res.set_content("Empty", "text/plain");
}

void get(const httplib::Request& req, httplib::Response& res) {
  auto numbers = req.matches[1];
  res.status = http::Code::OK;
  res.set_content(numbers, "text/plain");
}

void put(const httplib::Request& req, httplib::Response& res) {}

void post(const httplib::Request& req, httplib::Response& res) {
  if (req.has_param("json")) {
    auto json = req.get_param_value("json");
    LOG(INFO) << json;

    res.status = http::Code::OK;
    res.set_content(json, "application/json");
  } else {
    res.status = http::Code::BadRequest;
  }
}

void remove(const httplib::Request& req, httplib::Response& res) {}

}  // namespace api::list