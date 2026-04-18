#pragma once
#include <memory>
#include "../../application/service/reading_service.hpp"
#include "../adapter/out/in_memory_reading_repository.hpp"
#include "../adapter/out/mock_notification_service.hpp"

namespace infrastructure::config {

  // DI Контейнер (сборщик зависимостей)
  class DependencyContainer {
  private:
    std::shared_ptr<application::service::ReadingService> reading_service;

  public:
    DependencyContainer() {
      // 1. Создаем адаптеры
      auto repo = std::make_shared<adapter::out::InMemoryReadingRepository>();
      auto notif = std::make_shared<adapter::out::MockNotificationService>();

      // 2. Внедряем адаптеры в сервис (DI)
      reading_service = std::make_shared<application::service::ReadingService>(repo, notif);
    }

    std::shared_ptr<application::service::ReadingService> get_reading_service() {
      return reading_service;
    }
  };

}
