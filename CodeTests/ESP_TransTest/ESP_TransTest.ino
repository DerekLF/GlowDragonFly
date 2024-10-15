#include <esp_now.h>
#include <WiFi.h>

// Global integer variables
uint8_t data[6];
// MAC addresses of the 5 receiver ESPs
uint8_t broadcastAddress1[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC3 };
uint8_t broadcastAddress2[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC4 };
uint8_t broadcastAddress1[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC3 };
uint8_t broadcastAddress2[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC4 };
uint8_t broadcastAddress1[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC3 };
uint8_t broadcastAddress2[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC4 };
uint8_t bcA[][];
// Add remaining MAC addresses...

// Function to send data
void sendData(uint8_t* sData[6]) {
  calculateChecksum(sData);
  // Send data to each receiver
  esp_now_send(broadcastAddress1, (uint8_t*)sData, sizeof(sData));
  esp_now_send(broadcastAddress2, (uint8_t*)sData, sizeof(sData));
  // Repeat for other receivers...
}

void calculateChecksum(unint8_t* parseData[6]) {
  unint16_t sum = 0;
  for (int i = 0; i < 4; i++) {
    sum += parseData[i];
  }
  parseData[4] = sum & 0xFF;         // First byte of checksum
  parseData[5] = (sum >> 8) & 0xFF;  // Second byte of checksum
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // You may need to add the peers (like in the previous examples) here

  // Send the data
  sendData(data);
}

void loop() {
  // Optionally, continue
