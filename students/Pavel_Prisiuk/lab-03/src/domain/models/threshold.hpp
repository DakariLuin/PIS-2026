#pragma once
#include <stdexcept>

namespace domain::models {

  // Value Object: Порог срабатывания
  class Threshold {
  private:
    double min_val;
    double max_val;

  public:
    Threshold(double min, double max) : min_val(min), max_val(max) {
      // Инвариант: Минимум всегда меньше Максимума
      if (min >= max) {
        throw std::invalid_argument("Threshold Min must be less than Max");
      }
    }

    bool is_violated(double value) const {
      return value < min_val || value > max_val;
    }

    double get_min() const { return min_val; }
    double get_max() const { return max_val; }

    // Сравнение по значению
    bool operator==(const Threshold& other) const {
      return min_val == other.min_val && max_val == other.max_val;
    }
  };
}
