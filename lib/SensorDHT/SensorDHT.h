#ifndef SENSOR_DHT_H
#define SENSOR_DHT_H
#include <Arduino.h>
#include <DHT.h>

struct DhtReadings {
  float temperature = NAN; // NAN = Not a Number
  float humidity = NAN;
};

class SensorDHT {
public:
  SensorDHT(uint8_t pin, uint8_t type);
  void begin();
  DhtReadings read();
private:
  DHT dht_sensor;
  bool initialized = false;
};
#endif