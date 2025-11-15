#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H
#include <Arduino.h>
class PumpControl {
public:
  PumpControl(uint8_t pin);
  void begin();
  void control(bool turnOn); 
  bool getStatus() const;
private:
  uint8_t _pin;
  bool currentStatus = false;
};
#endif