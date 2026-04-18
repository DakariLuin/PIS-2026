#pragma once
#include <pqxx/pqxx>
#include <application/query/dto/sensor_dashboard_dto.hpp>

namespace application::query::handlers {
  class GetDashboardHandler {
  private:
    std::string conn_str;
  public:
    GetDashboardHandler(std::string str) : conn_str(str) {}
    dto::SensorDashboardDto handle(const std::string& id) {
      pqxx::connection C(conn_str);
      pqxx::nontransaction N(C);
      auto res = N.exec("SELECT sensor_name, is_active, last_value, total_alerts_count FROM sensor_dashboard_view WHERE sensor_id = " + N.quote(id));
      if (res.empty()) throw std::runtime_error("Not found in view");
      auto row = res[0];
      return { id, row[0].as<std::string>(), row[1].as<bool>(), row[2].as<double>(), row[3].as<int>() };
    }
  };
}
