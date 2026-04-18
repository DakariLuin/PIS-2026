#pragma once
#include <string>

namespace application::query::dto {
  struct SensorDashboardDto {
    std::string id;
    std::string name;
    bool is_active;
    double last_value;
    int alert_count;
  };
}
