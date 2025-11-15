#include "PumpControl.h"
PumpControl::PumpControl(uint8_t pin) : _pin(pin) {}
void PumpControl::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  currentStatus = false;
  Serial.println("Pump Control Initialized. Pump OFF.");
}
void PumpControl::control(bool turnOn) {
  if (turnOn != currentStatus) {
    digitalWrite(_pin, turnOn ? HIGH : LOW);
    currentStatus = turnOn;
    Serial.print("PUMP: ");
    Serial.println(currentStatus ? "ON" : "OFF");
  }
}
bool PumpControl::getStatus() const {
  return currentStatus;
}