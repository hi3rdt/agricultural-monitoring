
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Import các thư viện tùy chỉnh từ /lib
#include "Config.h"
#include "WifiConnect.h"
#include "Sensors.h"
#include "PumpControl.h"

// Khởi tạo các đối tượng toàn cục
WiFiClientSecure espClient; 
PubSubClient client(espClient);

// Biến trạng thái toàn cục
bool pumpStatus = false;
String currentMode = "automatic";
int lowThreshold = 30;
int highThreshold = 70;

// Biến thời gian
unsigned long previousMillis = 0;
unsigned long previousSoilCheck = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("----------------------------------------");
  Serial.print("Nhận được lệnh từ topic: ");
  Serial.println(topic);

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Nội dung: ");
  Serial.println(message);

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("Lỗi parse JSON lệnh: ");
    Serial.println(error.c_str());
    return;
  }

  // Cập nhật các biến toàn cục
  currentMode = doc["mode"] | "automatic";
  lowThreshold = doc["low_threshold"] | 30;
  highThreshold = doc["high_threshold"] | 70;
  bool serverPumpStatus = doc["pump_status"] | false;

  Serial.print("Trạng thái nhận: Mode=");
  Serial.print(currentMode);
  Serial.print(", Low=");
  Serial.print(lowThreshold);
  Serial.print("%, High=");
  Serial.print(highThreshold);
  Serial.println("%");

  if (currentMode == "manual") {
    Serial.print("Chế độ Manual -> Server muốn bơm: ");
    Serial.println(serverPumpStatus ? "ON" : "OFF");
    ctrlPump(serverPumpStatus, pumpStatus); // Gọi hàm từ thư viện PumpControl
  } else {
    Serial.println("Chế độ Automatic -> Bơm được checkAutoMode() quản lý.");
  }
  Serial.println("----------------------------------------");
}

/**
 * @brief Tự động kết nối lại MQTT nếu bị mất kết nối.
 * 
 */
void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("\nĐang kết nối MQTT (TLS Port 8883)...");
    String clientId = "esp32-agri-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("Đã kết nối!");
      client.subscribe(CONTROL_TOPIC);
      Serial.print("Đã đăng ký (subscribe) topic: ");
      Serial.println(CONTROL_TOPIC);
    } else {
      Serial.print("Lỗi, mã lỗi=");
      Serial.print(client.state());
      Serial.println(" Thử lại sau 5 giây...");
      delay(5000);
    }
  }
}


void sendDataToMQTT(float temperature, float humidity, float soilMoisture) {
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    Serial.println("WiFi hoặc MQTT bị ngắt kết nối! Bỏ qua gửi dữ liệu.");
    return;
  }

  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["soil"] = soilMoisture;
  doc["pump_status"] = pumpStatus; 

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(SENSOR_TOPIC, jsonBuffer);
  
  Serial.println("Đã gửi dữ liệu qua MQTT:");
  Serial.println(jsonBuffer);
}

// --- HÀM SETUP ---
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║ F ESP32 Agriculture (MQTTS)  ║");
  Serial.println("║       Version 4.1            ║");
  Serial.println("╚════════════════════════════════╝");
  
  setup_pump();    // Gọi từ lib/PumpControl
  setup_sensors(); // Gọi từ lib/Sensors
  setup_wifi_manager(); // Gọi từ lib/WifiConnect
  
  espClient.setInsecure(); 
  client.setServer(MQTT_BROKER, MQTT_PORT); 
  client.setCallback(callback); 
}

// --- LOOP ---
void loop() {
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi lost! Đang thử kết nối lại...");
    WiFi.reconnect(); 
    delay(5000); 
    return; 
  }
  
  if (!client.connected()) {
    reconnect_mqtt(); 
  }
  client.loop(); 

  // Luôn kiểm tra logic bơm tự động (mỗi 1 giây)
  if (currentMillis - previousSoilCheck >= SOIL_CHECK_INTERVAL) {
    previousSoilCheck = currentMillis;
    float currentSoil = read_soil_moisture(); 
    checkAutoMode(currentSoil, currentMode, lowThreshold, highThreshold, pumpStatus); 
  }

  // Gửi dữ liệu lên server (mỗi 5 giây)
  if (currentMillis - previousMillis >= SEND_INTERVAL) {
    previousMillis = currentMillis;
    Serial.println("\n========================================");
    
    float temperature, humidity, soilMoisture;
    read_sensors(temperature, humidity, soilMoisture); // Đọc tất cả cảm biến
    
    Serial.printf("Đọc cảm biến: T=%.1f°C, H=%.1f%%, Soil=%.1f%%\n", 
                  temperature, humidity, soilMoisture);
    
    sendDataToMQTT(temperature, humidity, soilMoisture);
    
    Serial.println("========================================\n");
  }
}