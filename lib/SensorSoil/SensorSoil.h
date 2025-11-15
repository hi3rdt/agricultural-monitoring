#ifndef SENSOR_SOIL_H
#define SENSOR_SOIL_H
#include <Arduino.h>
class SensorSoil {
public:
  SensorSoil(uint8_t pin);
  void begin();
  float readPercentage();
private:
  uint8_t _pin;
  const int ADC_MIN = 0;
  const int ADC_MAX = 4095;
};
#endif