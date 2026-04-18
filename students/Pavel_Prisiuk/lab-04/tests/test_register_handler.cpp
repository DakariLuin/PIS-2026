#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <application/command/handlers/register_reading_handler.hpp>
#include <infrastructure/adapter/out/mock_notification_service.hpp>

using namespace application::command;
using namespace application::command::handlers;
using ::testing::_;

class MockRepo : public application::port::out::ReadingRepository {
public:
  MOCK_METHOD(void, save, (const domain::models::Sensor&), (override));
  MOCK_METHOD(std::shared_ptr<domain::models::Sensor>, find_by_id, (const std::string&), (override));
};

TEST(RegisterHandlerTest, SuccessfullyProcessesReading) {
  auto repo = std::make_shared<MockRepo>();
  auto notifier = std::make_shared<infrastructure::adapter::out::MockNotificationService>();
  RegisterReadingHandler handler(repo, notifier);

  auto sensor = std::make_shared<domain::models::Sensor>("T-101", "Test", domain::models::Threshold(10, 30));

  EXPECT_CALL(*repo, find_by_id("T-101")).WillOnce(::testing::Return(sensor));
  EXPECT_CALL(*repo, save(_)).Times(1);

  handler.handle(RegisterReadingCommand("T-101", 25.0));
}
