#pragma once
#include <memory>
#include <application/service/reading_service_impl.hpp>
// ПОМЕНЯЛИ ИНКЛУД:
#include <infrastructure/adapter/out/postgres_reading_repository.hpp> 
#include <infrastructure/adapter/out/mock_notification_service.hpp>

namespace infrastructure::config {
  class DependencyContainer {
  private:
    std::shared_ptr<::application::service::ReadingServiceImpl> service;
  public:
    DependencyContainer() {
      std::string db_url = "postgresql://postgres:password@localhost:5432/smarthome";
      auto repo = std::make_shared<adapter::out::PostgresReadingRepository>(db_url);

      auto notif = std::make_shared<adapter::out::MockNotificationService>();

      auto reg_h = std::make_shared<::application::command::handlers::RegisterReadingHandler>(repo, notif);
      auto get_h = std::make_shared<::application::query::handlers::GetSensorHandler>(repo);

      service = std::make_shared<::application::service::ReadingServiceImpl>(reg_h, get_h);
    }
    std::shared_ptr<::application::port::in::RegisterReadingUseCase> get_service() { return service; }
  };
}
