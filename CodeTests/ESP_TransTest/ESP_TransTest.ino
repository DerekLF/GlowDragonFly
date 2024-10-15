#include <esp_now.h>
#include <WiFi.h>

// Structure to store the data
typedef struct struct_message {
  int values[6];
} struct_message;

struct_message myData;

// MAC addresses of the 5 receiver ESPs
uint8_t broadcastAddress1[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC3 };
uint8_t broadcastAddress2[] = { 0x24, 0x6F, 0x28, 0xA1, 0xB2, 0xC4 };
// Add remaining MAC addresses...

// Function to send data
void sendData() {
  myData.values[0] = 10;  // Populate with values
  myData.values[1] = 20;
  myData.values[2] = 30;
  myData.values[3] = 40;
  myData.values[4] = 50;
  myData.values[5] = 50;

  // Send data to each receiver
  esp_now_send(broadcastAddress1, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
  // Repeat for other receivers...
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



  ConnectPeer(broadcastAddress1);
  ConnectPeer(broadcastAddress2);

  // Send the data
  sendData();
}

esp_now_peer_info_t peerInfo;

int ConnectPeer(uint8_t *broadcastAddress) {
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Try to add the peer and return success or failure
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return 0;  // Failure
  } else {
    Serial.println("Peer added successfully");
    return 1;  // Success
  }
}

void loop() {
  // Keep sending data every 5 seconds
  delay(5000);
  sendData();
}