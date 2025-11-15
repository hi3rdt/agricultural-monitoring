#ifndef CONFIG_H
#define CONFIG_H



// --- ESP32 Master Config ---
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define SOIL_PIN 34
#define PUMP_PIN 2
#define AP_NAME_MASTER "ESP32-Agri-Setup"
#define MASTER_SEND_INTERVAL 5000
#define MASTER_SOIL_CHECK_INTERVAL 1000

// --- ESP32 CAM Config ---
#define CAM_FLASH_PIN 4
#define CAM_WIFI_SSID "WIFI 5"
#define CAM_WIFI_PASS "0987654322"
#define CAM_UPLOAD_URL "http://192.168.34.100:8080/api/upload-image-raw/"
#define CAM_COMMAND_URL "http://192.168.34.100:8080/api/cam-command"
#define CAM_COMMAND_INTERVAL 2000

// --- MQTT Config ---
#define MQTT_BROKER "e081335f47cb4fb78a222c0bca0ac487.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USER "tuduchiep123"
#define MQTT_PASS "Tuduchiep1405"
#define MQTT_SENSOR_TOPIC "agri/sensor"
#define MQTT_CONTROL_TOPIC "agri/status"
#endif