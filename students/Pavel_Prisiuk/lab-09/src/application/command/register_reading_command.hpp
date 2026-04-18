#pragma once
#include <string>

namespace application::command {
  struct RegisterReadingCommand {
    std::string sensor_id;
    double value;
    RegisterReadingCommand(std::string id, double v) : sensor_id(id), value(v) {}
  };
}
