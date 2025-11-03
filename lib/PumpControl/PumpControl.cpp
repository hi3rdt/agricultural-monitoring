#include "PumpControl.h"

PumpControl::PumpControl(uint8_t pin) : _pin(pin) {}

void PumpControl::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  currentStatus = false;
  Serial.println("Pump Control Initialized. Pump OFF.");
}

bool PumpControl::controlManual(bool turnOn) {
  if (turnOn != currentStatus) {
    digitalWrite(_pin, turnOn ? HIGH : LOW);
    currentStatus = turnOn;
    Serial.print("Manual Control: Pump turned ");
    Serial.println(currentStatus ? "ON" : "OFF");
  }
  return currentStatus;
}

bool PumpControl::controlAuto(float currentSoil, int lowThreshold, int highThreshold) {
  bool shouldBeOn = currentStatus;

  if (currentSoil < lowThreshold && !currentStatus) {
    shouldBeOn = true;
    Serial.printf("Auto: Soil %.1f%% < Low (%d%%). Pump ON\n", currentSoil, lowThreshold);
  } else if (currentSoil > highThreshold && currentStatus) {
    shouldBeOn = false;
    Serial.printf("Auto: Soil %.1f%% > High (%d%%). Pump OFF\n", currentSoil, highThreshold);
  }

  if (shouldBeOn != currentStatus) {
    digitalWrite(_pin, shouldBeOn ? HIGH : LOW);
    currentStatus = shouldBeOn;
  }
  return currentStatus;
}

bool PumpControl::getStatus() const {
  return currentStatus;
}