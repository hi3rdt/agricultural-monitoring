#include "HttpClient.h"

HttpClient::HttpClient() {}

bool HttpClient::sendSensorData(const char* url, float temp, float humid, float soil, bool pumpStatus) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("HTTP: WiFi disconnected. Send failed.");
    return false;
  }
  StaticJsonDocument<200> doc;
  if (!isnan(temp)) doc["temperature"] = temp;
  if (!isnan(humid)) doc["humidity"] = humid;
  if (soil >= 0) doc["soil"] = soil;
  doc["pump_status"] = pumpStatus;

  String jsonBuffer;
  serializeJson(doc, jsonBuffer);

  http.begin(url);
  http.setTimeout(10000);
  http.addHeader("Content-Type", "application/json");

  Serial.print("HTTP POST (Sensor): "); Serial.println(jsonBuffer);
  int httpCode = http.POST(jsonBuffer);
  bool success = (httpCode == HTTP_CODE_OK || httpCode == 201);

  if (!success) {
    Serial.printf("HTTP POST failed, Error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  return success;
}

DynamicJsonDocument* HttpClient::fetchJson(const char* url, size_t maxJsonSize) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("HTTP: WiFi disconnected. Fetch failed.");
    return nullptr;
  }
  http.begin(url);
  http.setTimeout(5000); // 5s timeout cho GET
  Serial.print("HTTP GET: "); Serial.println(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("HTTP Response: " + payload);
    DynamicJsonDocument* doc = new DynamicJsonDocument(maxJsonSize);
    DeserializationError error = deserializeJson(*doc, payload);
    if (error) {
      Serial.print("JSON Parse Error: "); Serial.println(error.c_str());
      delete doc;
      http.end();
      return nullptr;
    }
    http.end();
    return doc; // Thành công! Người gọi phải delete doc.
  } else {
    Serial.printf("HTTP GET failed, Error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return nullptr;
  }
}

bool HttpClient::uploadImage(const char* url, camera_fb_t* fb) {
   if (!fb) {
       Serial.println("HTTP: Invalid frame buffer.");
       return false;
   }
   if (WiFi.status() != WL_CONNECTED) {
       Serial.println("HTTP: WiFi disconnected. Upload failed.");
       return false;
   }

   http.begin(url);
   http.addHeader("Content-Type", "image/jpeg");
   http.setTimeout(15000); // 15s timeout cho upload

   Serial.printf("HTTP POST (Image): %zu bytes to %s\n", fb->len, url);
   int httpResponseCode = http.POST(fb->buf, fb->len);
   bool success = (httpResponseCode == HTTP_CODE_OK);

   if (success) {
       Serial.println("Image upload success.");
       // String response = http.getString();
       // Serial.println("Server Response: " + response);
   } else {
       Serial.printf("Image upload failed, Error: %s\n", http.errorToString(httpResponseCode).c_str());
   }
   http.end();
   return success;
}

bool HttpClient::testConnection(const char* url) {
   if (WiFi.status() != WL_CONNECTED) {
       Serial.println("HTTP: WiFi disconnected. Test failed.");
       return false;
   }
   http.begin(url);
   http.setTimeout(5000);
   Serial.print("HTTP Test: "); Serial.println(url);
   int httpCode = http.GET();
   bool success = (httpCode == HTTP_CODE_OK);
   if (success) {
       Serial.println("Server connection OK.");
   } else {
       Serial.printf("Server connection Failed, Error: %s\n", http.errorToString(httpCode).c_str());
   }
   http.end();
   return success;
}