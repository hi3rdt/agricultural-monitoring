#include "WifiConnect.h"
WifiConnect::WifiConnect() {}

void WifiConnect::setupWiFiManager(const char* apName) {
  delay(10);
  Serial.println("\n========== WIFI MANAGER SETUP ==========");
  WiFiManager wm;
  Serial.println("Đang khởi động WiFi Manager...");
  Serial.printf("Nếu chưa có WiFi, hãy kết nối vào AP '%s'\n", apName);
  wm.setConfigPortalTimeout(180);
  if (!wm.autoConnect(apName)) {
    Serial.println("Cấu hình thất bại/hết giờ. Khởi động lại...");
    delay(3000);
    ESP.restart();
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP: "); Serial.println(WiFi.localIP());
  Serial.println("================================\n");
}

void WifiConnect::setupHardcoded(const char* ssid, const char* pass) {
  delay(10);
  Serial.print("Đang kết nối Wi-Fi (Hardcoded): ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối Wi-Fi!");
  Serial.print("ESP32 IP: "); Serial.println(WiFi.localIP());
}

bool WifiConnect::isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

void WifiConnect::reconnectWiFi() {
  // Dùng cho chế độ hardcoded (CAM)
  Serial.println("Mất kết nối Wi-Fi, đang thử kết nối lại...");
  WiFi.disconnect();
  WiFi.reconnect();
}