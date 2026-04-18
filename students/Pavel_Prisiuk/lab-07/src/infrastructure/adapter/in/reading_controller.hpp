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
      // POST - Write Model
      CROW_ROUTE(app, "/api/readings").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body) return crow::response(400, "Invalid JSON");
        try {
          application::port::in::RegisterReadingCommand cmd{
              json_body["sensor_id"].s(), json_body["value"].d()
          };
          return crow::response(201, use_case->register_reading(cmd));
        }
        catch (const std::exception& e) {
          return crow::response(400, e.what());
        }
          });

      // GET - Dashboard (Read Model / CQRS)
      CROW_ROUTE(app, "/api/dashboard/<string>")
        ([this](const std::string& id) {
        try {
          auto dto = use_case->get_sensor_dashboard(id);
          crow::json::wvalue res;
          res["id"] = dto.id;
          res["name"] = dto.name;
          res["is_active"] = dto.is_active;
          res["last_value"] = dto.last_value;
          res["alert_count"] = dto.alert_count;
          return crow::response(200, res);
        }
        catch (const std::exception& e) {
          return crow::response(404, e.what());
        }
          });

      // GET - Basic Info
      CROW_ROUTE(app, "/api/sensors/<string>")
        ([this](const std::string& id) {
        try {
          auto dto = use_case->get_sensor_info(id);
          crow::json::wvalue res;
          res["id"] = dto.id;
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
