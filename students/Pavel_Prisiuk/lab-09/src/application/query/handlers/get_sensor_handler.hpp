#pragma once
#include <memory>
#include <application/query/dto/sensor_dto.hpp>
#include <application/port/out/reading_repository.hpp>

namespace application::query::handlers {
  class GetSensorHandler {
  private:
    std::shared_ptr<port::out::ReadingRepository> repo;
  public:
    GetSensorHandler(std::shared_ptr<port::out::ReadingRepository> r) : repo(r) {}
    dto::SensorDto handle(const std::string& id) {
      auto s = repo->find_by_id(id);
      if (!s) throw std::runtime_error("Not found");
      return { s->get_id(), s->get_active(), s->get_active() ? "OK" : "DISABLED" };
    }
  };
}
