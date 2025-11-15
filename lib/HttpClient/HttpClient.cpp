#include "HttpClient.h"

HttpClient::HttpClient() {}

bool HttpClient::getCommand(const char* url, JsonDocument& doc) {
  http.begin(url);
  int httpResponseCode = http.GET();
  bool gotCaptureCmd = false;

  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("HTTP Response (Command): " + payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("JSON Parse Error: "); Serial.println(error.c_str());
    } else {
      const char* command = doc["command"];
      if (command && strcmp(command, "capture") == 0) {
        gotCaptureCmd = true;
      }
    }
  } else {
    Serial.printf("HTTP GET failed, Error: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  http.end();
  return gotCaptureCmd;
}

bool HttpClient::uploadImage(const char* url, camera_fb_t* fb) {
   if (!fb) return false;
   http.begin(url);
   http.addHeader("Content-Type", "image/jpeg");
   http.setTimeout(15000); 
   int httpResponseCode = http.POST(fb->buf, fb->len);
   bool success = (httpResponseCode == HTTP_CODE_OK);
   if (success) {
       Serial.printf("Gửi ảnh thành công! Mã HTTP: %d\n", httpResponseCode);
   } else {
       Serial.printf("Gửi ảnh thất bại! Mã lỗi: %s\n", http.errorToString(httpResponseCode).c_str());
   }
   http.end();
   return success;
}