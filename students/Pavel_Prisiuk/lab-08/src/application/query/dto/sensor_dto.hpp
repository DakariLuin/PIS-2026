#pragma once
#include <string>

namespace application::query::dto {
  struct SensorDto {
    std::string id;
    bool is_active;
    std::string status_msg;
  };
}
