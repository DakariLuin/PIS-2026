import grpc
import smarthome_pb2
import smarthome_pb2_grpc

def run():
    # 1. Подключаемся к твоему C++ серверу
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = smarthome_pb2_grpc.SmartHomeServiceStub(channel)

        # 2. Тестируем Unary RPC (запрос-ответ)
        print("--- Тестируем GetSensorStatus ---")
        request = smarthome_pb2.SensorStatusRequest(sensor_id="T-101")
        try:
            response = stub.GetSensorStatus(request)
            print(f"Сервер ответил: ID={response.id}, Active={response.is_active}, Msg={response.status_msg}")
        except grpc.RpcError as e:
            print(f"Ошибка: {e.code()} - {e.details()}")

        # 3. Тестируем Server Streaming (поток данных)
        print("\n--- Тестируем StreamReadings (стрим) ---")
        stream_request = smarthome_pb2.StreamRequest(sensor_id="T-101")
        try:
            for update in stub.StreamReadings(stream_request):
                print(f"Обновление от сервера: Датчик {update.sensor_id}, Значение: {update.value:.2f}")
        except grpc.RpcError as e:
            print(f"Ошибка стрима: {e.code()}")

if __name__ == '__main__':
    run()