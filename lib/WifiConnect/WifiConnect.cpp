#include "WifiConnect.h"

WifiConnect::WifiConnect(const char* apName) : _apName(apName) {}

WifiConnect::WifiConnect(const char* apName, char* uploadUrlBuf, char* commandUrlBuf) :
  _apName(apName), _uploadUrlBuffer(uploadUrlBuf), _commandUrlBuffer(commandUrlBuf) {}

void WifiConnect::setupWiFi() {
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);

  // Chỉ thêm parameter nếu là CAM
  if (_uploadUrlBuffer && _commandUrlBuffer) {
      // Sao chép giá trị mặc định vào buffer trước
      strncpy(_uploadUrlBuffer, DEFAULT_CAM_UPLOAD_URL, MAX_URL_LENGTH);
      strncpy(_commandUrlBuffer, DEFAULT_CAM_COMMAND_URL, MAX_URL_LENGTH);

      // Cấp phát động cho parameters
      custom_upload_url_param = new WiFiManagerParameter("upload_url", "Upload URL", _uploadUrlBuffer, MAX_URL_LENGTH);
      custom_command_url_param = new WiFiManagerParameter("command_url", "Command URL", _commandUrlBuffer, MAX_URL_LENGTH);

      wm.addParameter(custom_upload_url_param);
      wm.addParameter(custom_command_url_param);
      Serial.println("WiFiManager: Đã thêm các trường URL tùy chỉnh.");
  }

  Serial.printf("Đang kết nối WiFi hoặc mở AP '%s'...\n", _apName);
  if (!wm.autoConnect(_apName)) {
    Serial.println("Cấu hình thất bại/hết giờ. Khởi động lại...");
    delay(3000);
    ESP.restart();
  }

  // Nếu là CAM, lấy giá trị đã lưu/nhập
  if (_uploadUrlBuffer && _commandUrlBuffer && custom_upload_url_param && custom_command_url_param) {
      strcpy(_uploadUrlBuffer, custom_upload_url_param->getValue());
      strcpy(_commandUrlBuffer, custom_command_url_param->getValue());
  }

  Serial.println("\nWiFi đã kết nối!");
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  if (_uploadUrlBuffer) { Serial.print("Upload URL: "); Serial.println(_uploadUrlBuffer); }
  if (_commandUrlBuffer) { Serial.print("Command URL: "); Serial.println(_commandUrlBuffer); }
  Serial.println("=====================================\n");
  
  // (Lưu ý: custom_upload_url_param và custom_command_url_param sẽ bị rò rỉ bộ nhớ
  // nếu không được delete. WiFiManager nên được cải tiến để xử lý việc này,
  // nhưng với ESP32 khởi động lại, nó tạm chấp nhận được)
}

bool WifiConnect::isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

void WifiConnect::reconnectWiFi() {
  Serial.println("Đang thử kết nối lại WiFi...");
  WiFi.reconnect();
}

const char* WifiConnect::getUploadURL() const {
  return _uploadUrlBuffer ? _uploadUrlBuffer : "";
}

const char* WifiConnect::getCommandURL() const {
  return _commandUrlBuffer ? _commandUrlBuffer : "";
}