#pragma once
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <string>
#include <iostream>
#include <memory>

namespace infrastructure::messaging {
  class RabbitMQPublisher {
  private:
    amqp_connection_state_t conn;
    std::string exchange = "smarthome_events";
  public:
    RabbitMQPublisher(const std::string& host) {
      conn = amqp_new_connection();
      amqp_socket_t* socket = amqp_tcp_socket_new(conn);
      if (amqp_socket_open(socket, host.c_str(), 5672)) {
        return;
      }
      amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "admin", "password");
      amqp_channel_open(conn, 1);
      amqp_exchange_declare(conn, 1, amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes("topic"), 0, 1, 0, 0, amqp_empty_table);
      std::cout << "✅ [AMQP] Connected to " << host << std::endl;
    }

    void publish(const std::string& routing_key, const std::string& message) {
      amqp_basic_properties_t props;
      props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
      props.content_type = amqp_cstring_bytes("application/json");
      props.delivery_mode = 2;

      amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        0, 0, &props, amqp_cstring_bytes(message.c_str()));
      std::cout << "📤 [AMQP SENT] " << routing_key << std::endl;
    }

    ~RabbitMQPublisher() {
      if (conn) {
        amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
      }
    }
  };
}
