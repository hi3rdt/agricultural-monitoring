#ifndef HTTP_CLIENT_WRAPPER_H
#define HTTP_CLIENT_WRAPPER_H
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_camera.h"
class HttpClient {
public:
  HttpClient();
  // Trả về true nếu nhận được lệnh "capture"
  bool getCommand(const char* url, JsonDocument& doc); 
  bool uploadImage(const char* url, camera_fb_t* fb);
private:
  HTTPClient http;
};
#endif