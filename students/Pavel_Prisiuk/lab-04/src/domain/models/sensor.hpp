#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <domain/models/threshold.hpp>
#include <domain/events/sensor_events.hpp>

namespace domain::models {
  class Sensor {
  private:
    std::string id, name;
    bool is_active;
    Threshold threshold;
    std::vector<std::shared_ptr<::domain::events::DomainEvent>> events;
  public:
    Sensor(std::string id, std::string name, Threshold t) : id(id), name(name), threshold(t), is_active(true) {}

    void process_reading(double val) {
      if (!is_active) throw std::runtime_error("Sensor inactive");
      if (val < -100.0 || val > 200.0) throw std::invalid_argument("Physical limit");
      if (threshold.is_violated(val)) {
        double limit = (val > threshold.get_max()) ? threshold.get_max() : threshold.get_min();
        events.push_back(std::make_shared<::domain::events::AlertTriggered>(id, val, limit));
      }
    }
    void deactivate(const std::string& reason) {
      is_active = false;
      events.push_back(std::make_shared<::domain::events::SensorDeactivated>(id, reason));
    }
    std::string get_id() const { return id; }
    bool get_active() const { return is_active; }
    const std::vector<std::shared_ptr<::domain::events::DomainEvent>>& get_events() const { return events; }
    void clear_events() { events.clear(); }
  };
}
