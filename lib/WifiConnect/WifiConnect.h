#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
class WifiConnect {
public:
  WifiConnect();
  void setupWiFiManager(const char* apName);
  void setupHardcoded(const char* ssid, const char* pass);
  bool isConnected();
  void reconnectWiFi(); 
};
#endif