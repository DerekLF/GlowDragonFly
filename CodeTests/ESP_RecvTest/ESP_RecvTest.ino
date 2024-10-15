#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    int values[5];
} struct_message;

struct_message incomingData;

// Callback when data is received
void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingData, incomingData, sizeof(incomingData));
    Serial.print("Data received: ");
    for (int i = 0; i < 5; i++) {
        Serial.print(incomingData.values[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register for incoming data
    esp_now_register_recv_cb(onDataRecv);
}

void loop() {}
    // Keep running the loop
