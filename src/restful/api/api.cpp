#include "api.h"

namespace http::api {
void lists_get(const httplib::Request& req, httplib::Response& res) {
  auto numbers = req.matches[1];
  res.set_content(numbers, "text/plain");
}

void lists_post(const httplib::Request& req, httplib::Response& res) {
  std::string body = req.body;
  res.set_content(body, "application/json");
}

}  // namespace http::api