#include "CameraControl.h"

CameraControl::CameraControl(uint8_t flashPin) : _flashPin(flashPin) {}

bool CameraControl::begin() {
  pinMode(_flashPin, OUTPUT);
  digitalWrite(_flashPin, LOW);
  Serial.println("Flash Pin Initialized.");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    initialized = false;
    return false;
  }
  initialized = true;
  Serial.println("Camera Initialized Successfully.");
  return true;
}

void CameraControl::captureAndUpload(const char* uploadUrl, HttpClient& httpClient) {
  if (!initialized) {
    Serial.println("Error: Camera not initialized.");
    return;
  }

  Serial.println("Capturing image with flash...");
  digitalWrite(_flashPin, HIGH);
  unsigned long flash_on_time = millis();
  while (millis() - flash_on_time < 150) { yield(); } // Non-blocking wait

  camera_fb_t* fb = esp_camera_fb_get();
  digitalWrite(_flashPin, LOW);

  if (!fb) {
    Serial.println("Failed to capture image!");
    return;
  }

  // Giao việc upload cho HttpClient
  httpClient.uploadImage(uploadUrl, fb);

  // Giải phóng bộ nhớ
  esp_camera_fb_return(fb);
}