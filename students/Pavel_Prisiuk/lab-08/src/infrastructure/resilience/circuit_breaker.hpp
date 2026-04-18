#pragma once
#include <chrono>
#include <atomic>
#include <iostream>

namespace infrastructure::resilience {
  enum class State { CLOSED, OPEN };

  class CircuitBreaker {
  private:
    std::atomic<State> state{ State::CLOSED };
    std::atomic<int> failures{ 0 };
    const int threshold = 3;
  public:
    bool can_execute() { return state == State::CLOSED; }

    void record_failure() {
      failures++;
      if (failures >= threshold) {
        state = State::OPEN;
        std::cerr << "🔴 [Circuit Breaker] OPENED! Blocking external calls." << std::endl;
      }
    }

    void record_success() {
      failures = 0;
      state = State::CLOSED;
    }

    std::string status() { return (state == State::CLOSED) ? "CLOSED" : "OPEN"; }
  };
}
