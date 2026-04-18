#pragma once
#include <memory>
#include <application/port/in/register_reading_use_case.hpp>
#include <application/command/handlers/register_reading_handler.hpp>
#include <application/query/handlers/get_sensor_handler.hpp>

namespace application::service {
  class ReadingServiceImpl : public port::in::RegisterReadingUseCase {
  private:
    std::shared_ptr<command::handlers::RegisterReadingHandler> reg_h;
    std::shared_ptr<query::handlers::GetSensorHandler> get_h;
  public:
    ReadingServiceImpl(std::shared_ptr<command::handlers::RegisterReadingHandler> rh, std::shared_ptr<query::handlers::GetSensorHandler> gh) : reg_h(rh), get_h(gh) {}
    std::string register_reading(const port::in::RegisterReadingCommand& cmd) override;
    query::dto::SensorDto get_sensor_info(const std::string& id) override;
  };
}
