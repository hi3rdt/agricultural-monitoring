#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H
#include <Arduino.h>

class PumpControl {
public:
  PumpControl(uint8_t pin);
  void begin();
  bool controlManual(bool turnOn);
  bool controlAuto(float currentSoil, int lowThreshold, int highThreshold);
  bool getStatus() const;
private:
  uint8_t _pin;
  bool currentStatus = false;
};
#endif