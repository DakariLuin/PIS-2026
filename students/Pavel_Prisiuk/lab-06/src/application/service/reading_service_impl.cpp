#include <application/service/reading_service_impl.hpp>

namespace application::service {
  std::string ReadingServiceImpl::register_reading(const port::in::RegisterReadingCommand& cmd) {
    reg_h->handle({ cmd.sensor_id, cmd.value });
    return "SUCCESS";
  }
  query::dto::SensorDto ReadingServiceImpl::get_sensor_info(const std::string& id) {
    return get_h->handle(id);
  }
}
