#pragma once
#include <memory>
#include "../port/in/register_reading_use_case.hpp"
#include "../port/out/reading_repository.hpp"
#include "../port/out/notification_service.hpp"

namespace application::service {

  // Сервис бизнес-логики. Реализует входящий порт.
  class ReadingService : public port::in::RegisterReadingUseCase {
  private:
    // Зависимости (исходящие порты) хранятся как умные указатели
    std::shared_ptr<port::out::ReadingRepository> repository;
    std::shared_ptr<port::out::NotificationService> notifications;
    std::shared_ptr<query::handlers::GetDashboardHandler> dashboard_handler;

  public:
    // Constructor Dependency Injection
    ReadingService(std::shared_ptr<port::out::ReadingRepository> repo,
      std::shared_ptr<port::out::NotificationService> notif);

    std::string register_reading(const port::in::RegisterReadingCommand& command) override;
  };

}
