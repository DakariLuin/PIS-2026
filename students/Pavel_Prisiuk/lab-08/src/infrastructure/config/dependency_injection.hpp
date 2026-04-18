#pragma once
#include <memory>
#include <application/service/reading_service_impl.hpp>
#include <infrastructure/adapter/out/postgres_reading_repository.hpp>
#include <infrastructure/adapter/out/mock_notification_service.hpp>
#include <infrastructure/messaging/rabbitmq_publisher.hpp>
#include <infrastructure/resilience/circuit_breaker.hpp>

namespace infrastructure::config {
  class DependencyContainer {
  private:
    std::shared_ptr<::application::service::ReadingServiceImpl> service;
  public:
    DependencyContainer() {
      std::string db_url = "host=127.0.0.1 port=5434 user=postgres password=password dbname=reading_db";

      auto repo = std::make_shared<adapter::out::PostgresReadingRepository>(db_url);
      auto notif = std::make_shared<adapter::out::MockNotificationService>();

      auto publisher = std::make_shared<::infrastructure::messaging::RabbitMQPublisher>("localhost");
      auto breaker = std::make_shared<::infrastructure::resilience::CircuitBreaker>();

      auto reg_h = std::make_shared<::application::command::handlers::RegisterReadingHandler>(repo, publisher, breaker);
      auto get_h = std::make_shared<::application::query::handlers::GetSensorHandler>(repo);
      auto dash_h = std::make_shared<::application::query::handlers::GetDashboardHandler>(db_url);

      service = std::make_shared<::application::service::ReadingServiceImpl>(reg_h, get_h, dash_h);
    }
    std::shared_ptr<::application::service::ReadingServiceImpl> get_service() { return service; }
  };
}
