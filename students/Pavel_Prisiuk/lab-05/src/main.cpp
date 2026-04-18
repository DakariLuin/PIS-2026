#include <crow.h>
#include <infrastructure/config/dependency_injection.hpp>
#include <infrastructure/adapter/in/reading_controller.hpp>

int main() {
  infrastructure::config::DependencyContainer di;
  auto service = di.get_service();
  crow::SimpleApp app;
  infrastructure::adapter::in::ReadingController controller(app, service);
  app.port(8080).multithreaded().run();
  return 0;
}
