#include "SensorDHT.h"

SensorDHT::SensorDHT(uint8_t pin, uint8_t type) : dht_sensor(pin, type) {}

void SensorDHT::begin() {
  dht_sensor.begin();
  initialized = true;
  Serial.println("DHT Sensor Initialized.");
}

DhtReadings SensorDHT::read() {
  DhtReadings readings;
  if (!initialized) {
     Serial.println("Error: DHT sensor not initialized.");
     return readings;
  }
  readings.temperature = dht_sensor.readTemperature();
  readings.humidity = dht_sensor.readHumidity();
  
  if (isnan(readings.temperature) || isnan(readings.humidity)) {
    Serial.println("Warning: Failed to read from DHT sensor!");
    // Giữ nguyên logic cũ của bạn (trả về 0)
    readings.temperature = 0.0f;
    readings.humidity = 0.0f;
  }
  return readings;
}