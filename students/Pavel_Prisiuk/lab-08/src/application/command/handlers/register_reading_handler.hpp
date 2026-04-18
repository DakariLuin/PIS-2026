#pragma once
#include <memory>
#include <application/command/register_reading_command.hpp>
#include <application/port/out/reading_repository.hpp>
#include <application/port/out/notification_service.hpp>
#include <infrastructure/messaging/rabbitmq_publisher.hpp>
#include <infrastructure/resilience/circuit_breaker.hpp>

namespace application::command::handlers {
  class RegisterReadingHandler {
  private:
    std::shared_ptr<port::out::ReadingRepository> repo;
    std::shared_ptr<::infrastructure::messaging::RabbitMQPublisher> publisher;
    std::shared_ptr<::infrastructure::resilience::CircuitBreaker> breaker;
  public:
    RegisterReadingHandler(
      std::shared_ptr<port::out::ReadingRepository> r,
      std::shared_ptr<::infrastructure::messaging::RabbitMQPublisher> p,
      std::shared_ptr<::infrastructure::resilience::CircuitBreaker> b)
      : repo(r), publisher(p), breaker(b) {
    }

    void handle(const RegisterReadingCommand& cmd) {
      auto sensor = repo->find_by_id(cmd.sensor_id);
      if (!sensor) throw std::runtime_error("Sensor not found");

      sensor->process_reading(cmd.value);
      repo->save(*sensor);

      // Асинхронная отправка события через Circuit Breaker
      if (breaker->can_execute()) {
        try {
          std::string msg = "{\"sensor_id\":\"" + cmd.sensor_id + "\",\"val\":" + std::to_string(cmd.value) + "}";
          publisher->publish("sensor.reading", msg);

          for (auto& event : sensor->get_events()) {
            if (std::dynamic_pointer_cast<domain::events::AlertTriggered>(event)) {
              publisher->publish("sensor.alert", msg);
            }
          }
          breaker->record_success();
        }
        catch (...) {
          breaker->record_failure();
        }
      }
      sensor->clear_events();
    }
  };
}
