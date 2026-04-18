#pragma once
#include <memory>
#include <application/command/register_reading_command.hpp>
#include <application/port/out/reading_repository.hpp>
#include <application/port/out/notification_service.hpp>

namespace application::command::handlers {
  class RegisterReadingHandler {
  private:
    std::shared_ptr<port::out::ReadingRepository> repo;
    std::shared_ptr<port::out::NotificationService> notifier;
  public:
    RegisterReadingHandler(std::shared_ptr<port::out::ReadingRepository> r, std::shared_ptr<port::out::NotificationService> n) : repo(r), notifier(n) {}

    void handle(const RegisterReadingCommand& cmd) {
      auto sensor = repo->find_by_id(cmd.sensor_id);
      if (!sensor) throw std::runtime_error("Sensor not found");
      sensor->process_reading(cmd.value);
      repo->save(*sensor);
      for (auto& event : sensor->get_events()) {
        if (std::dynamic_pointer_cast<domain::events::AlertTriggered>(event)) {
          notifier->send_alert(cmd.sensor_id, "CRITICAL VALUE!");
        }
      }
      sensor->clear_events();
    }
  };
}
