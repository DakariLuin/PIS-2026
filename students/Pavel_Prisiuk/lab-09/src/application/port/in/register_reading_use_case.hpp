#pragma once
#include <string>
#include <application/query/dto/sensor_dto.hpp>
#include <application/query/dto/sensor_dashboard_dto.hpp>

namespace application::port::in {
  struct RegisterReadingCommand { std::string sensor_id; double value; };

  class RegisterReadingUseCase {
  public:
    virtual ~RegisterReadingUseCase() = default;
    virtual std::string register_reading(const RegisterReadingCommand& cmd) = 0;
    virtual query::dto::SensorDto get_sensor_info(const std::string& id) = 0;
    virtual query::dto::SensorDashboardDto get_sensor_dashboard(const std::string& id) = 0;
  };
}
