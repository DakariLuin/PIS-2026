#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <application/command/handlers/register_reading_handler.hpp>
#include <infrastructure/adapter/out/mock_notification_service.hpp>
#include <infrastructure/projection/sensor_projection_handler.hpp>
#include <domain/models/sensor.hpp>

using namespace application::command;
using namespace application::command::handlers;
using ::testing::_;
using ::testing::Return;

// Mock репозитория для изоляции хендлера
class MockRepo : public application::port::out::ReadingRepository {
public:
  MOCK_METHOD(void, save, (const domain::models::Sensor&), (override));
  MOCK_METHOD(std::shared_ptr<domain::models::Sensor>, find_by_id, (const std::string&), (override));
};

TEST(RegisterHandlerTest, SuccessfullyProcessesReading) {
  auto repo = std::make_shared<MockRepo>();
  auto notifier = std::make_shared<infrastructure::adapter::out::MockNotificationService>();

  // Создаем проекцию-заглушку (stub), так как в этом юнит-тесте нас не интересует БД Read Model
  auto projection = std::make_shared<infrastructure::projection::SensorProjectionHandler>("host=127.0.0.1 user=postgres password=password dbname=smarthome");

  // Теперь передаем 3 аргумента в конструктор
  RegisterReadingHandler handler(repo, notifier, projection);

  // Подготовка тестовых данных
  auto threshold = domain::models::Threshold(10.0, 40.0);
  auto sensor = std::make_shared<domain::models::Sensor>("T-101", "Test Sensor", threshold);

  // Настраиваем ожидания для мока
  EXPECT_CALL(*repo, find_by_id("T-101")).WillOnce(Return(sensor));
  EXPECT_CALL(*repo, save(_)).Times(1);

  // Вызов действия
  handler.handle(RegisterReadingCommand("T-101", 25.0));

  // Тест пройдет успешно, если хендлер вызвал repo->save()
}
