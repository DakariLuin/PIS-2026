#pragma once
#include <string>

namespace domain {
  // Value Object: Показание с датчика
  struct Reading {
    std::string sensor_id;
    double value;

    Reading(std::string id, double val) : sensor_id(std::move(id)), value(val) {}
  };

}
