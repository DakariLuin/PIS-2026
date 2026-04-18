#pragma once
#include <string>

namespace application::port::in {

  // DTO команды регистрации показания
  struct RegisterReadingCommand {
    std::string sensor_id;
    double value;
  };

  // Входящий порт: Интерфейс для регистрации показания
  class RegisterReadingUseCase {
  public:
    virtual ~RegisterReadingUseCase() = default;
    virtual std::string register_reading(const RegisterReadingCommand& command) = 0;
  };

}
