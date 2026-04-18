#include <gtest/gtest.h>
#include <domain/models/sensor.hpp> // Путь относительно папки src

using namespace domain::models;

// Тест инварианта Threshold
TEST(ThresholdTest, InvalidRangeThrowsException) {
  EXPECT_THROW(Threshold(30.0, 20.0), std::invalid_argument);
}

// Тест инварианта Sensor: Физические пределы
TEST(SensorTest, PhysicalLimitInvariant) {
  Threshold t(15.0, 25.0);
  Sensor s("T-101", "Living Room", t);

  EXPECT_THROW(s.process_reading(500.0), std::invalid_argument);
}

// Тест регистрации доменного события
TEST(SensorTest, AlertEventIsRegistered) {
  Threshold t(10.0, 30.0);
  Sensor s("T-101", "Kitchen", t);

  s.process_reading(35.0); // Превышение

  auto events = s.get_events();
  EXPECT_EQ(events.size(), 1);

  // Используем полное имя пространства имен для надежности
  auto alert = std::dynamic_pointer_cast<::domain::events::AlertTriggered>(events[0]);
  EXPECT_NE(alert, nullptr);
  EXPECT_DOUBLE_EQ(alert->value, 35.0);
}

// Тест состояния: Неактивный датчик
TEST(SensorTest, InactiveSensorInvariant) {
  Threshold t(10.0, 30.0);
  Sensor s("T-101", "Kitchen", t);
  s.deactivate("Manual shutdown");

  EXPECT_THROW(s.process_reading(20.0), std::runtime_error);
}
