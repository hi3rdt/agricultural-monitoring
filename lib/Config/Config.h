#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- Cấu hình chung ---
#define MAX_URL_LENGTH 100 // Kích thước buffer URL cho WiFiManager

// --- Cấu hình ESP32 Master ---
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define SOIL_PIN 34
#define PUMP_PIN 2

#define DEFAULT_MASTER_SENSOR_URL "https://agricultural-backend.onrender.com/sensor"
#define DEFAULT_MASTER_STATUS_URL "https://agricultural-backend.onrender.com/status"
#define AP_NAME_MASTER "ESP32-Agri-Setup"

#define MASTER_SEND_INTERVAL 5000       // Gửi dữ liệu mỗi 5s
#define MASTER_SOIL_CHECK_INTERVAL 1000 // Kiểm tra đất mỗi 1s

// --- Cấu hình ESP32 CAM ---
#define CAM_FLASH_PIN 4
#define AP_NAME_CAM "ESP32CamSetupAP"

// URL mặc định khi vào portal (bạn đã yêu cầu dùng IP local)
#define DEFAULT_CAM_UPLOAD_URL "http://192.168.34.100:8080/upload-image-raw/"
#define DEFAULT_CAM_COMMAND_URL "http://192.168.34.100:8080/api/cam-command"

#define CAM_COMMAND_INTERVAL 2000 // Hỏi lệnh mỗi 2s

#endif // CONFIG_H