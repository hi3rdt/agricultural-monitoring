#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H
#include <Arduino.h>
#include "esp_camera.h"
#include <HttpClient.h>

class CameraControl {
public:
  CameraControl(uint8_t flashPin);
  bool begin();
  void captureAndUpload(const char* uploadUrl, HttpClient& httpClient);

private:
  uint8_t _flashPin;
  bool initialized = false;
  
  // Định nghĩa chân AI Thinker
  static const uint8_t PWDN_GPIO_NUM = 32;
  static const uint8_t RESET_GPIO_NUM = -1;
  static const uint8_t XCLK_GPIO_NUM = 0;
  static const uint8_t SIOD_GPIO_NUM = 26;
  static const uint8_t SIOC_GPIO_NUM = 27;
  static const uint8_t Y9_GPIO_NUM = 35;
  static const uint8_t Y8_GPIO_NUM = 34;
  static const uint8_t Y7_GPIO_NUM = 39;
  static const uint8_t Y6_GPIO_NUM = 36;
  static const uint8_t Y5_GPIO_NUM = 21;
  static const uint8_t Y4_GPIO_NUM = 19;
  static const uint8_t Y3_GPIO_NUM = 18;
  static const uint8_t Y2_GPIO_NUM = 5;
  static const uint8_t VSYNC_GPIO_NUM = 25;
  static const uint8_t HREF_GPIO_NUM = 23;
  static const uint8_t PCLK_GPIO_NUM = 22;
};
#endif