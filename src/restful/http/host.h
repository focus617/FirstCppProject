#pragma once

#include <json.hpp>
#include <string>
#include <unordered_set>

using json = nlohmann::json;

namespace http {
class Host {
 public:
  std::string ip{};
  uint port;
  std::unordered_set<std::string> bannedips;

  Host(const std::string& ipaddr, const uint pt) : ip{ipaddr}, port{pt} {}

  void setup(const json& config) {
    ip = config.at("ip");
    port = config.at("port");
    if (config.contains("bannedIps")) {
      bannedips = config.value("bannedIps", std::unordered_set<std::string>{});
    }
  }

  bool isBanned(const std::string& ipaddr) const {
    return bannedips.find(ipaddr) != bannedips.end();
  }

  void from_json(const json& j, Host& h) {
    h.ip = j.value("ip", "localhost");
    h.port = j.value("port", 8080);
    h.bannedips = j.value("bannedIps", std::unordered_set<std::string>{});
  }

  void to_json(json& j, const Host& h) {
    j = json{{"ip", h.ip}, {"port", h.port}, {"bannedIps", {}}};
  }
};
}  // namespace http