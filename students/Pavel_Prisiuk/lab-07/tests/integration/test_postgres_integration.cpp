#include <gtest/gtest.h>
#include <infrastructure/adapter/out/postgres_reading_repository.hpp>
#include <domain/models/sensor.hpp>
#include <domain/models/threshold.hpp>
#include <iostream>

using namespace infrastructure::adapter::out;
using namespace domain::models;

class PostgresIntegrationTest : public ::testing::Test {
protected:
  // Попробуем альтернативный формат строки подключения
  std::string conn_str = "host=127.0.0.1 port=5433 user=postgres password=password dbname=smarthome";

  void SetUp() override {
    try {
      pqxx::connection C(conn_str);
      pqxx::work W(C);
      W.exec("DELETE FROM sensors WHERE id LIKE 'TEST-%';");
      W.commit();
    }
    catch (const std::exception& e) {
      // ПЕЧАТАЕМ РЕАЛЬНУЮ ОШИБКУ
      std::cerr << "\n[DATABASE ERROR] " << e.what() << std::endl;
      GTEST_SKIP() << "PostgreSQL not available: " << e.what();
    }
  }
};

TEST_F(PostgresIntegrationTest, SaveAndFindSensorWorks) {
  PostgresReadingRepository repo(conn_str);
  Threshold t(10.0, 40.0);
  auto sensor = std::make_shared<Sensor>("TEST-001", "Integration-Sensor", t);
  ASSERT_NO_THROW(repo.save(*sensor));
  auto found = repo.find_by_id("TEST-001");
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->get_id(), "TEST-001");
}

TEST_F(PostgresIntegrationTest, FindNonExistentReturnsNull) {
  PostgresReadingRepository repo(conn_str);
  auto found = repo.find_by_id("NON-EXISTENT-ID");
  EXPECT_EQ(found, nullptr);
}
