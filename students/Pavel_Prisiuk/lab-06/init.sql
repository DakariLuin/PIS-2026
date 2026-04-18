-- Таблица датчиков
CREATE TABLE IF NOT EXISTS sensors (
    id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    min_threshold DOUBLE PRECISION NOT NULL,
    max_threshold DOUBLE PRECISION NOT NULL
);

-- Инициализируем тестовый датчик
INSERT INTO sensors (id, name, is_active, min_threshold, max_threshold)
VALUES ('T-101', 'Living Room', true, 10.0, 30.0)
ON CONFLICT (id) DO NOTHING;