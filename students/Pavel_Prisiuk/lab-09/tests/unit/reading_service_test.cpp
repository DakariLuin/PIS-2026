#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <application/service/reading_service_impl.hpp>

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
    auto publisher = std::make_shared<infrastructure::messaging::RabbitMQPublisher>("localhost");
    auto breaker = std::make_shared<infrastructure::resilience::CircuitBreaker>();

    auto reg_h = std::make_shared<application::command::handlers::RegisterReadingHandler>(repo, publisher, breaker);
    auto get_h = std::make_shared<application::query::handlers::GetSensorHandler>(repo);
    auto dash_h = std::make_shared<application::query::handlers::GetDashboardHandler>("mock_url");

    service = std::make_shared<application::service::ReadingServiceImpl>(reg_h, get_h, dash_h);
  }
};

TEST_F(ReadingServiceIntegrationTest, ServiceDelegatesRegisterToHandler) {
  auto sensor = std::make_shared<domain::models::Sensor>("T-101", "Test", domain::models::Threshold(10, 40));
  EXPECT_CALL(*repo, find_by_id("T-101")).WillOnce(::testing::Return(sensor));
  EXPECT_CALL(*repo, save(::testing::_)).Times(1);
  service->register_reading({ "T-101", 25.5 });
}
