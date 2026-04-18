#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <domain/models/threshold.hpp>        // Относительный путь от src
#include <domain/events/sensor_events.hpp>    // Относительный путь от src

namespace domain::models {

  class Sensor {
  private:
    std::string id;
    std::string name;
    bool is_active;
    Threshold safety_threshold;
    std::vector<std::shared_ptr<::domain::events::DomainEvent>> domain_events;

  public:
    Sensor(std::string id, std::string name, Threshold threshold)
      : id(id), name(name), safety_threshold(threshold), is_active(true) {
    }

    void process_reading(double value) {
      if (!is_active) throw std::runtime_error("Sensor is inactive");
      if (value < -100.0 || value > 200.0) throw std::invalid_argument("Physical limit exceeded");

      if (safety_threshold.is_violated(value)) {
        double limit = (value > safety_threshold.get_max())
          ? safety_threshold.get_max()
          : safety_threshold.get_min();

        domain_events.push_back(std::make_shared<::domain::events::AlertTriggered>(id, value, limit));
      }
    }

    void deactivate() { is_active = false; }
    const std::vector<std::shared_ptr<::domain::events::DomainEvent>>& get_events() const { return domain_events; }
    void clear_events() { domain_events.clear(); }
    std::string get_id() const { return id; }
  };

  void deactivate(const std::string& reason) {
    is_active = false;
    // Регистрируем новое событие изменения состояния
    domain_events.push_back(std::make_shared<::domain::events::SensorDeactivated>(id, reason));
  }
}
