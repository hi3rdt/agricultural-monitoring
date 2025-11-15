#ifndef MQTT_CLIENT_WRAPPER_H
#define MQTT_CLIENT_WRAPPER_H
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Config.h> // Để lấy MQTT defines

class MqttClient {
public:
  MqttClient(Client& netClient);
  void setup(PubSubClient::callback_t callback);
  void connect();
  void loop();
  bool isConnected();
  void publishSensorData(float temp, float humid, float soil, bool pumpStatus);

private:
  PubSubClient _client;
  const char* _broker = MQTT_BROKER;
  const int _port = MQTT_PORT;
  const char* _user = MQTT_USER;
  const char* _pass = MQTT_PASS;
};
#endif