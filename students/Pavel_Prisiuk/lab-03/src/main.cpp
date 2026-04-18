#include "crow.h"
#include "infrastructure/config/dependency_injection.hpp"
#include "infrastructure/adapter/in/reading_controller.hpp"

int main() {
  // Инициализация DI-контейнера (создает UseCase и Адаптеры БД/TG)
  infrastructure::config::DependencyContainer container;
  auto reading_service = container.get_reading_service();

  // Создаем Crow App
  crow::SimpleApp app;

  // Инициализируем REST Контроллер, передавая ему App и UseCase
  infrastructure::adapter::in::ReadingController controller(app, reading_service);

  std::cout << "🚀 Запуск Reading Service на порту 8080...\n";

  // Запускаем сервер
  app.port(8080).multithreaded().run();

  return 0;
}
