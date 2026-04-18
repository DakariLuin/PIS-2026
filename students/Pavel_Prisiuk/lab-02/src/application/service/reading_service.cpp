#include "reading_service.hpp"
#include "../../domain/reading.hpp"
#include <string>

namespace application::service {

  ReadingService::ReadingService(std::shared_ptr<port::out::ReadingRepository> repo,
    std::shared_ptr<port::out::NotificationService> notif)
    : repository(std::move(repo)), notifications(std::move(notif)) {
  }

  std::string ReadingService::register_reading(const port::in::RegisterReadingCommand& command) {
    // 1. Создаем доменный объект
    domain::Reading reading(command.sensor_id, command.value);

    // 2. Сохраняем через исходящий порт (БД)
    repository->save(reading);

    // 3. Бизнес-правило: если перегрев (> 30.0), отправляем алерт
    if (command.value > 30.0) {
      std::string message = "Внимание! Перегрев: " + std::to_string(command.value) + "°C";
      notifications->send_alert(command.sensor_id, message);
    }

    return "SUCCESS";
  }

}
