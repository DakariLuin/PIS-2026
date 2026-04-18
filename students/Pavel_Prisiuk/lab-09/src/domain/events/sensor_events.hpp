#pragma once
#include <string>
#include <memory>

namespace domain::events {
  struct DomainEvent { virtual ~DomainEvent() = default; };

  struct AlertTriggered : public DomainEvent {
    std::string sensor_id;
    double value;
    double threshold_limit;
    AlertTriggered(std::string id, double val, double limit) : sensor_id(id), value(val), threshold_limit(limit) {}
  };

  struct SensorDeactivated : public DomainEvent {
    std::string sensor_id;
    std::string reason;
    SensorDeactivated(std::string id, std::string r) : sensor_id(id), reason(r) {}
  };
}
