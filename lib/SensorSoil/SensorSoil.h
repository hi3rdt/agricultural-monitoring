#ifndef SENSOR_SOIL_H
#define SENSOR_SOIL_H
#include <Arduino.h>

class SensorSoil {
public:
  SensorSoil(uint8_t pin);
  void begin();
  float readPercentage();
  float getLastReadingPercentage() const;
private:
  uint8_t _pin;
  float lastReading = -1.0;
  // (Hiệu chỉnh 2 giá trị này dựa trên cảm biến thực tế)
  const int ADC_MIN = 0;    // Giá trị ADC khi khô
  const int ADC_MAX = 4095; // Giá trị ADC khi ướt
};
#endif