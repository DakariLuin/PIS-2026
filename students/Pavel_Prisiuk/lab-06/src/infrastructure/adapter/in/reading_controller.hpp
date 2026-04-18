#pragma once
#include <crow.h>
#include <application/port/in/register_reading_use_case.hpp>
#include <memory>
#include <string>

namespace infrastructure::adapter::in {

  class ReadingController {
  private:
    std::shared_ptr<application::port::in::RegisterReadingUseCase> use_case;

  public:
    ReadingController(crow::SimpleApp& app, std::shared_ptr<application::port::in::RegisterReadingUseCase> uc)
      : use_case(std::move(uc))
    {
      // 1. POST - Создать показание (Command)
      CROW_ROUTE(app, "/api/readings").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body) return crow::response(400, "Invalid JSON");

        try {
          application::port::in::RegisterReadingCommand cmd{
              json_body["sensor_id"].s(), json_body["value"].d()
          };
          std::string result = use_case->register_reading(cmd);

          crow::json::wvalue res;
          res["status"] = "success"; res["message"] = result;
          return crow::response(201, res);
        }
        catch (const std::exception& e) {
          return crow::response(404, e.what()); // Например "Sensor not found"
        }
          });

      // 2. GET - Получить информацию о датчике (Query)
      CROW_ROUTE(app, "/api/sensors/<string>").methods(crow::HTTPMethod::GET)
        ([this](const std::string& sensor_id) {
        try {
          auto dto = use_case->get_sensor_info(sensor_id);

          crow::json::wvalue res;
          res["id"] = dto.id;
          res["is_active"] = dto.is_active;
          res["status"] = dto.status_msg;
          return crow::response(200, res);
        }
        catch (const std::exception& e) {
          return crow::response(404, e.what());
        }
          });
    }
  };
}
