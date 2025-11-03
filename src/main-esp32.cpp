#include <Arduino.h>
#include <Config.h>       // File cấu hình
#include <WifiConnect.h>
#include <SensorDHT.h>
#include <SensorSoil.h>
#include <PumpControl.h>
#include <HttpClient.h>

// --- Khởi tạo các đối tượng thư viện ---
WifiConnect wifiHandler(AP_NAME_MASTER);
SensorDHT dhtSensor(DHT_PIN, DHT_TYPE);
SensorSoil soilSensor(SOIL_PIN);
PumpControl pump(PUMP_PIN);
HttpClient httpClient;

// --- Biến trạng thái và thời gian ---
String currentMode = "automatic";
int lowThreshold = 30;
int highThreshold = 70;
bool currentPumpStatus = false;

unsigned long previousSendMillis = 0;
unsigned long previousSoilCheck = 0;

// --- Hàm Setup ---
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- ESP32 Master Node ---");

    dhtSensor.begin();
    soilSensor.begin();
    pump.begin();
    wifiHandler.setupWiFi();

    delay(2000);
    httpClient.testConnection(DEFAULT_MASTER_STATUS_URL);
}

// --- Hàm Loop ---
void loop() {
    unsigned long currentMillis = millis();

    // 1. Đảm bảo có WiFi
    if (!wifiHandler.isConnected()) {
        wifiHandler.reconnectWiFi();
        delay(5000); // Chờ 5s
        return;
    }

    // 2. Kiểm tra đất và điều khiển tự động (mỗi 1s)
    if (currentMillis - previousSoilCheck >= MASTER_SOIL_CHECK_INTERVAL) {
        previousSoilCheck = currentMillis;
        if (currentMode == "automatic") {
            float soilMoisture = soilSensor.readPercentage();
            if (soilMoisture >= 0) {
                currentPumpStatus = pump.controlAuto(soilMoisture, lowThreshold, highThreshold);
            }
        }
    }

    // 3. Đọc cảm biến, gửi dữ liệu, lấy trạng thái (mỗi 5s)
    if (currentMillis - previousSendMillis >= MASTER_SEND_INTERVAL) {
        previousSendMillis = currentMillis;
        Serial.println("\n--- Processing Interval ---");

        // Lấy trạng thái từ server TRƯỚC
        DynamicJsonDocument* statusDoc = httpClient.fetchJson(DEFAULT_MASTER_STATUS_URL);
        if (statusDoc) {
            currentMode = (*statusDoc)["mode"] | "automatic";
            lowThreshold = (*statusDoc)["low_threshold"] | 30;
            highThreshold = (*statusDoc)["high_threshold"] | 70;
            bool serverPumpStatus = (*statusDoc)["pump_status"] | false;

            Serial.printf("Received Settings: Mode=%s, Thresh=%d-%d, SrvPump=%s\n",
                          currentMode.c_str(), lowThreshold, highThreshold, serverPumpStatus ? "ON" : "OFF");

            if (currentMode == "manual") {
                currentPumpStatus = pump.controlManual(serverPumpStatus);
            }
            delete statusDoc; // Giải phóng bộ nhớ
        } else {
             Serial.println("Failed to fetch status, using last known settings.");
        }

        // Đọc cảm biến
        DhtReadings dhtData = dhtSensor.read();
        float soilMoisture = soilSensor.getLastReadingPercentage();
        currentPumpStatus = pump.getStatus(); // Lấy trạng thái bơm MỚI NHẤT

        Serial.printf("Sensor Read: T=%.1f C, H=%.1f %%, Soil=%.1f %%\n",
                  dhtData.temperature, dhtData.humidity, soilMoisture);

        // Gửi dữ liệu
        httpClient.sendSensorData(DEFAULT_MASTER_SENSOR_URL, dhtData.temperature, dhtData.humidity, soilMoisture, currentPumpStatus);
    }
}