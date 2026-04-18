#pragma once
#include "crow.h"
#include "../../../application/port/in/register_reading_use_case.hpp"
#include <memory>
#include <string>

namespace infrastructure::adapter::in {

  class ReadingController {
  private:
    std::shared_ptr<application::port::in::RegisterReadingUseCase> use_case;

  public:
    // Принимаем приложение Crow по ссылке, чтобы зарегистрировать в нем маршруты
    ReadingController(crow::SimpleApp& app, std::shared_ptr<application::port::in::RegisterReadingUseCase> uc)
      : use_case(std::move(uc))
    {
      // Настройка маршрута POST /api/readings
      CROW_ROUTE(app, "/api/readings").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        // Парсим JSON
        auto json_body = crow::json::load(req.body);
        if (!json_body) {
          return crow::response(400, "Invalid JSON");
        }

        try {
          // Извлекаем данные
          std::string sensor_id = json_body["sensor_id"].s();
          double value = json_body["value"].d();

          // Вызываем Входящий Порт (Application Layer)
          application::port::in::RegisterReadingCommand cmd{ sensor_id, value };
          std::string result = use_case->register_reading(cmd);

          // Успешный ответ
          crow::json::wvalue res_json;
          res_json["status"] = "success";
          res_json["message"] = result;
          return crow::response(201, res_json);

        }
        catch (const std::exception& e) {
          return crow::response(400, "Bad Request: Missing required fields");
        }
          });
    }
  };

}
