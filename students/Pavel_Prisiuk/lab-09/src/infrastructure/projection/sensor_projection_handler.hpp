#pragma once
#include <pqxx/pqxx>
#include <string>

namespace infrastructure::projection {
  class SensorProjectionHandler {
  private:
    std::string conn_str;
  public:
    SensorProjectionHandler(std::string str) : conn_str(str) {}

    void on_reading_processed(const std::string& id, double val) {
      pqxx::connection C(conn_str);
      pqxx::work W(C);
      W.exec("UPDATE sensor_dashboard_view SET last_value = " + W.quote(val) + ", last_update = NOW() WHERE sensor_id = " + W.quote(id));
      W.commit();
    }

    void on_alert_triggered(const std::string& id) {
      pqxx::connection C(conn_str);
      pqxx::work W(C);
      W.exec("UPDATE sensor_dashboard_view SET total_alerts_count = total_alerts_count + 1 WHERE sensor_id = " + W.quote(id));
      W.commit();
    }
  };
}
