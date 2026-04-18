import requests
import time

BASE_URL = "http://localhost:8080"

def test_full_sensor_flow():
    # 1. Проверяем, что датчик T-101 существует (Query)
    resp = requests.get(f"{BASE_URL}/api/sensors/T-101")
    assert resp.status_code == 200
    assert resp.json()["id"] == "T-101"

    # 2. Отправляем нормальное показание (Command)
    payload = {"sensor_id": "T-101", "value": 25.0}
    resp = requests.post(f"{BASE_URL}/api/readings", json=payload)
    assert resp.status_code == 201
    
    # 3. Отправляем критическое показание (Вызывает алерт в логах сервера)
    payload = {"sensor_id": "T-101", "value": 55.0}
    resp = requests.post(f"{BASE_URL}/api/readings", json=payload)
    assert resp.status_code == 201
    assert resp.json()["status"] == "success"

if __name__ == "__main__":
    try:
        test_full_sensor_flow()
        print("✅ E2E Test Passed!")
    except Exception as e:
        print(f"❌ E2E Test Failed: {e}")