#include "SensorSoil.h"
SensorSoil::SensorSoil(uint8_t pin) : _pin(pin) {}
void SensorSoil::begin() { Serial.println("Soil Sensor (Analog) Initialized."); }
float SensorSoil::readPercentage() {
  int soilRaw = analogRead(_pin);
  float percentage = map(soilRaw, ADC_MIN, ADC_MAX, 100, 0);
  percentage = constrain(percentage, 0, 100);
  return percentage;
}