#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <application/service/reading_service_impl.hpp>
#include <infrastructure/adapter/out/mock_notification_service.hpp>

using ::testing::_;

// Поддельный репозиторий для тестов сервиса
class MockRepoForService : public application::port::out::ReadingRepository {
public:
  MOCK_METHOD(void, save, (const domain::models::Sensor&), (override));
  MOCK_METHOD(std::shared_ptr<domain::models::Sensor>, find_by_id, (const std::string&), (override));
};

class ReadingServiceIntegrationTest : public ::testing::Test {
protected:
  std::shared_ptr<MockRepoForService> repo;
  std::shared_ptr<application::service::ReadingServiceImpl> service;

  void SetUp() override {
    repo = std::make_shared<MockRepoForService>();
    auto notifier = std::make_shared<infrastructure::adapter::out::MockNotificationService>();

    // Создаем хендлеры
    auto reg_h = std::make_shared<application::command::handlers::RegisterReadingHandler>(repo, notifier);
    auto get_h = std::make_shared<application::query::handlers::GetSensorHandler>(repo);

    // Собираем сервис-фасад
    service = std::make_shared<application::service::ReadingServiceImpl>(reg_h, get_h);
  }
};

TEST_F(ReadingServiceIntegrationTest, ServiceDelegatesRegisterToHandler) {
  application::port::in::RegisterReadingCommand cmd{ "T-101", 25.5 };

  auto sensor = std::make_shared<domain::models::Sensor>("T-101", "Test", domain::models::Threshold(10, 40));

  EXPECT_CALL(*repo, find_by_id("T-101")).WillOnce(::testing::Return(sensor));
  EXPECT_CALL(*repo, save(_)).Times(1);

  std::string result = service->register_reading(cmd);
  EXPECT_EQ(result, "SUCCESS");
}

TEST_F(ReadingServiceIntegrationTest, ServiceDelegatesQueryToHandler) {
  auto sensor = std::make_shared<domain::models::Sensor>("T-101", "Test", domain::models::Threshold(10, 40));
  EXPECT_CALL(*repo, find_by_id("T-101")).WillOnce(::testing::Return(sensor));

  auto dto = service->get_sensor_info("T-101");
  EXPECT_EQ(dto.id, "T-101");
  EXPECT_TRUE(dto.is_active);
}
