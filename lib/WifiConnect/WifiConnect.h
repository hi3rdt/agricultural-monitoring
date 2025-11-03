#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H
#include <Arduino.h>
#include <WiFiManager.h>
#include <Config.h> // Để lấy MAX_URL_LENGTH

class WifiConnect {
public:
  // Constructor cho Master (chỉ cần tên AP)
  WifiConnect(const char* apName);
  // Constructor cho CAM (cần tên AP và buffer để lưu URL)
  WifiConnect(const char* apName, char* uploadUrlBuf, char* commandUrlBuf);

  void setupWiFi();
  bool isConnected();
  void reconnectWiFi();

  // Chỉ dùng cho CAM
  const char* getUploadURL() const;
  const char* getCommandURL() const;

private:
  const char* _apName;
  char* _uploadUrlBuffer = nullptr;
  char* _commandUrlBuffer = nullptr;
  // Các đối tượng Parameter phải được quản lý cẩn thận
  WiFiManagerParameter* custom_upload_url_param = nullptr;
  WiFiManagerParameter* custom_command_url_param = nullptr;
};
#endif