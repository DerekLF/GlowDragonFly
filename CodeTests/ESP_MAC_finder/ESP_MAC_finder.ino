#include <WiFi.h>  // Use <ESP8266WiFi.h> for ESP8266

void setup() {
  Serial.begin(115200);

  // Initialize WiFi (station mode is required for ESP-NOW)
  WiFi.mode(WIFI_STA);
  WiFi.begin();

  // Get the MAC Address
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address: ");
  Serial.println(macAddress);
}

void loop() {
  // Nothing to do here
}
