#pragma once
#include <stdexcept>

namespace domain::models {
  class Threshold {
  private:
    double min_val, max_val;
  public:
    Threshold(double min, double max) : min_val(min), max_val(max) {
      if (min >= max) throw std::invalid_argument("Min must be < Max");
    }
    bool is_violated(double val) const { return val < min_val || val > max_val; }
    double get_min() const { return min_val; }
    double get_max() const { return max_val; }
  };
}
