#include <Arduino.h>
#include <Config.h>
#include <WifiConnect.h>
#include <CameraControl.h>
#include <HttpClient.h>
#include <ArduinoJson.h>


WifiConnect wifiHandler;
CameraControl camera(CAM_FLASH_PIN);
HttpClient httpClient;


unsigned long last_command_check = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("--- KHỞI ĐỘNG ESP32-CAM (CHẾ ĐỘ CHỜ LỆNH) ---");

  if (!camera.begin()) {
    Serial.println("Lỗi camera! Khởi động lại...");
    delay(5000);
    ESP.restart();
  }
  
  
  wifiHandler.setupHardcoded(CAM_WIFI_SSID, CAM_WIFI_PASS);
}

void loop() {
  unsigned long current_millis = millis();
  
  if (current_millis - last_command_check >= CAM_COMMAND_INTERVAL) {
    last_command_check = current_millis;
    
    if (wifiHandler.isConnected()) {
      
      StaticJsonDocument<100> doc;
      bool capture = httpClient.getCommand(CAM_COMMAND_URL, doc);
      
      if (capture) {
        Serial.println(">>> NHẬN ĐƯỢC LỆNH CHỤP! <<<");
        camera.captureAndUpload(CAM_UPLOAD_URL, httpClient);
      }
      
    } else {
      wifiHandler.reconnectWiFi(); 
    }
  }
}