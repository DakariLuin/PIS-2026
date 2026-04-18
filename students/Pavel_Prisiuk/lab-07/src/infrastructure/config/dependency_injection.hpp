#pragma once
#include <memory>
#include <application/service/reading_service_impl.hpp>
#include <infrastructure/adapter/out/postgres_reading_repository.hpp>
#include <infrastructure/adapter/out/mock_notification_service.hpp>
#include <infrastructure/projection/sensor_projection_handler.hpp>

namespace infrastructure::config {
  class DependencyContainer {
  private:
    std::shared_ptr<::application::service::ReadingServiceImpl> service;
  public:
    DependencyContainer() {
      std::string db_url = "host=127.0.0.1 port=5433 user=postgres password=password dbname=smarthome";

      auto repo = std::make_shared<adapter::out::PostgresReadingRepository>(db_url);
      auto notif = std::make_shared<adapter::out::MockNotificationService>();
      auto proj = std::make_shared<::infrastructure::projection::SensorProjectionHandler>(db_url);

      auto reg_h = std::make_shared<::application::command::handlers::RegisterReadingHandler>(repo, notif, proj);
      auto get_h = std::make_shared<::application::query::handlers::GetSensorHandler>(repo);
      auto dash_h = std::make_shared<::application::query::handlers::GetDashboardHandler>(db_url);

      service = std::make_shared<::application::service::ReadingServiceImpl>(reg_h, get_h, dash_h);
    }
    std::shared_ptr<::application::port::in::RegisterReadingUseCase> get_service() { return service; }
  };
}
