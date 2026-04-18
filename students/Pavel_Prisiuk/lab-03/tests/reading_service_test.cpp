#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/application/service/reading_service.hpp"
#include "../src/application/port/out/reading_repository.hpp"
#include "../src/application/port/out/notification_service.hpp"

using ::testing::_;
using ::testing::HasSubstr;

// 1. Создаем Mock-классы для Исходящих портов
class MockReadingRepository : public application::port::out::ReadingRepository {
public:
  MOCK_METHOD(void, save, (const domain::Reading& reading), (override));
};

class MockNotificationService : public application::port::out::NotificationService {
public:
  MOCK_METHOD(void, send_alert, (const std::string& sensor_id, const std::string& message), (override));
};

// 2. Тестовый набор
class ReadingServiceTest : public ::testing::Test {
protected:
  std::shared_ptr<MockReadingRepository> mock_repo;
  std::shared_ptr<MockNotificationService> mock_notif;
  std::shared_ptr<application::service::ReadingService> service;

  void SetUp() override {
    mock_repo = std::make_shared<MockReadingRepository>();
    mock_notif = std::make_shared<MockNotificationService>();
    // Внедряем моки в реальный сервис
    service = std::make_shared<application::service::ReadingService>(mock_repo, mock_notif);
  }
};

// Тест 1: Нормальная температура (Алерт НЕ должен отправляться)
TEST_F(ReadingServiceTest, NormalTemperatureDoesNotTriggerAlert) {
  application::port::in::RegisterReadingCommand cmd{ "T-101", 24.5 };

  // Ожидаем, что БД вызовется ровно 1 раз
  EXPECT_CALL(*mock_repo, save(_)).Times(1);

  // Ожидаем, что отправка уведомлений НЕ вызовется
  EXPECT_CALL(*mock_notif, send_alert(_, _)).Times(0);

  std::string result = service->register_reading(cmd);
  EXPECT_EQ(result, "SUCCESS");
}

// Тест 2: Перегрев (Алерт ДОЛЖЕН отправиться)
TEST_F(ReadingServiceTest, CriticalTemperatureTriggersAlert) {
  application::port::in::RegisterReadingCommand cmd{ "T-101", 35.0 };

  // Ожидаем сохранение в БД
  EXPECT_CALL(*mock_repo, save(_)).Times(1);

  // Ожидаем 1 алерт с текстом о перегреве,
  EXPECT_CALL(*mock_notif, send_alert("T-101", HasSubstr("Перегрев"))).Times(1);

  std::string result = service->register_reading(cmd);
  EXPECT_EQ(result, "SUCCESS");
}
