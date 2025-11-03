#include "SensorSoil.h"

SensorSoil::SensorSoil(uint8_t pin) : _pin(pin) {}

void SensorSoil::begin() {
  // pinMode(_pin, INPUT); // Không cần cho chân analog
  Serial.println("Soil Sensor (Analog) Initialized.");
}

float SensorSoil::readPercentage() {
  int soilRaw = analogRead(_pin);
  // Đảo ngược map: 0 (khô) -> 100%, 4095 (ướt) -> 0%
  lastReading = map(soilRaw, ADC_MIN, ADC_MAX, 100, 0);
  lastReading = constrain(lastReading, 0, 100);
  return lastReading;
}

float SensorSoil::getLastReadingPercentage() const {
  return lastReading;
}