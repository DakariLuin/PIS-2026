-- Read Model: Таблица для быстрого чтения (Дашборд)
CREATE TABLE IF NOT EXISTS sensor_dashboard_view (
    sensor_id VARCHAR(50) PRIMARY KEY,
    sensor_name VARCHAR(100),
    is_active BOOLEAN,
    last_value DOUBLE PRECISION,
    last_update TIMESTAMP,
    total_alerts_count INTEGER DEFAULT 0 -- Предвычисленное поле
);

-- Индекс для оптимизации (CQRS Style)
CREATE INDEX idx_dashboard_active ON sensor_dashboard_view(is_active);