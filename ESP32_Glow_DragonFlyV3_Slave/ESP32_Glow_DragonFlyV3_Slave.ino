#include <esp_now.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

//change this to your hotspot..
const char* ssid = "";
const char* password = "";
//change this to be a unique name
const char* name = "SlaveESP1";

typedef struct usefulInfo {
  uint8_t animationMode;   // 1 byte
  int speed;               // 2 bytes
  uint8_t colorCombo;      // 1 byte
  unsigned long interval;  // 4 bytes
  uint64_t checksum;       // 8 bytes
} usefulInfo;

usefulInfo espData;

void setup() {
  Serial.begin(115200);

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname(name);
  ArduinoOTA.setPassword("Glow");
  delay(1000);

  //OTA Handler
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      unsigned int Download = (progress / (total / 100));
      Serial.printf("Progress: %u%%\r", progress);
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback function
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  ArduinoOTA.handle();
}

// Callback when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  if (len == sizeof(usefulInfo)) {
    // Copy incoming data into the espData structure
    memcpy(&espData, incomingData, sizeof(espData));

    // Print received data to Serial
    Serial.println("Data received:");
    Serial.print("Animation Mode: ");
    Serial.println(espData.animationMode);

    Serial.print("Speed: ");
    Serial.println(espData.speed);

    Serial.print("Color Combo: ");
    Serial.println(espData.colorCombo);

    Serial.print("Interval: ");
    Serial.println(espData.interval);

    Serial.print("Checksum: ");
    Serial.println((uint64_t)espData.checksum, HEX);
    Serial.println();
  } else {
    Serial.println("Received data of unexpected length.");
  }
}
