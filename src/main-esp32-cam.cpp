#include <Arduino.h>
#include <Config.h>       // File cấu hình
#include <WifiConnect.h>
#include <CameraControl.h>
#include <HttpClient.h>

// --- Buffer để lưu URL từ WiFiManager ---
char upload_url_buf[MAX_URL_LENGTH];
char command_url_buf[MAX_URL_LENGTH];

// --- Khởi tạo các đối tượng thư viện ---
WifiConnect wifiHandler(AP_NAME_CAM, upload_url_buf, command_url_buf);
CameraControl camera(CAM_FLASH_PIN);
HttpClient httpClient;

// --- Biến thời gian ---
unsigned long last_command_check = 0;

// --- Hàm Setup ---
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- ESP32 CAM Node ---");

    if (!camera.begin()) {
        Serial.println("Critical Camera Error! Restarting...");
        delay(5000);
        ESP.restart();
    }
    
    // Hàm này sẽ kết nối WiFi VÀ điền URL vào 2 biến buffer ở trên
    wifiHandler.setupWiFi();
}

// --- Hàm Loop ---
void loop() {
    unsigned long currentMillis = millis();

    // 1. Đảm bảo có WiFi
    if (!wifiHandler.isConnected()) {
        wifiHandler.reconnectWiFi();
        delay(5000);
        return;
    }

    // 2. Kiểm tra lệnh từ server (mỗi 2s)
    if (currentMillis - last_command_check >= CAM_COMMAND_INTERVAL) {
        last_command_check = currentMillis;

        // Lấy URL command đã được cấu hình từ WifiConnect
        const char* current_command_url = wifiHandler.getCommandURL();

        DynamicJsonDocument* commandDoc = httpClient.fetchJson(current_command_url);
        if (commandDoc) {
            const char* command = (*commandDoc)["command"];
            if (command && strcmp(command, "capture") == 0) {
                Serial.println(">>> Received Capture Command! <<<");
                
                // Lấy URL upload đã được cấu hình
                const char* current_upload_url = wifiHandler.getUploadURL();
                
                // Chụp và gửi ảnh
                camera.captureAndUpload(current_upload_url, httpClient);
            }
            delete commandDoc; // Giải phóng bộ nhớ
        } else {
             Serial.println("Failed to fetch command or parse JSON.");
        }
    }
}