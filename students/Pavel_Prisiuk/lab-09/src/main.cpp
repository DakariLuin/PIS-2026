#include <crow.h>
#include <grpcpp/grpcpp.h>
#include <thread>
#include "infrastructure/config/dependency_injection.hpp"
#include "infrastructure/adapter/in/reading_controller.hpp"
#include "smarthome.grpc.pb.h" // Сгенерируется автоматически

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using smarthome::SmartHomeService;

// Реализация логики gRPC
class SmartHomeGrpcService final : public SmartHomeService::Service {
  Status GetSensorStatus(ServerContext* context, const smarthome::SensorStatusRequest* request,
    smarthome::SensorStatusResponse* response) override {
    response->set_id(request->sensor_id());
    response->set_is_active(true);
    response->set_status_msg("Healthy (via gRPC Binary)");
    return Status::OK;
  }

  Status StreamReadings(ServerContext* context, const smarthome::StreamRequest* request,
    grpc::ServerWriter<smarthome::ReadingUpdate>* writer) override {
    for (int i = 0; i < 5; ++i) {
      smarthome::ReadingUpdate update;
      update.set_sensor_id(request->sensor_id());
      update.set_value(20.0 + i * 1.5);
      writer->Write(update); // Отправляем данные в поток
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return Status::OK;
  }
};

void RunGrpcServer() {
  std::string addr("0.0.0.0:50051");
  SmartHomeGrpcService service;
  ServerBuilder builder;
  builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "📡 gRPC Server listening on " << addr << std::endl;
  server->Wait();
}

int main() {
  // 1. Запуск gRPC в отдельном потоке
  std::thread grpc_thread(RunGrpcServer);
  grpc_thread.detach();

  // 2. Запуск Crow (REST)
  infrastructure::config::DependencyContainer di;
  auto service = di.get_service();
  crow::SimpleApp app;
  infrastructure::adapter::in::ReadingController controller(app, service);

  std::cout << "🚀 REST API Gateway listening on port 8080" << std::endl;
  app.port(8080).multithreaded().run();

  return 0;
}
