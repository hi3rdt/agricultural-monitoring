#ifndef HTTP_CLIENT_WRAPPER_H
#define HTTP_CLIENT_WRAPPER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_camera.h" // Cần cho kiểu camera_fb_t

class HTTPClient {
public:
  HttpClient();
  bool sendSensorData(const char* url, float temp, float humid, float soil, bool pumpStatus);
  DynamicJsonDocument* fetchJson(const char* url, size_t maxJsonSize = 1024);
  bool uploadImage(const char* url, camera_fb_t* fb);
  bool testConnection(const char* url);

private:
  HTTPClient http; 
};

#endif // HTTP_CLIENT_WRAPPER_H