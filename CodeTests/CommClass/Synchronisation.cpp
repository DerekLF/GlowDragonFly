#include "Synchronisation.h"

DragonComms::DragonComms(uint8_t* bcA[]){
  _broadcastAddress = bcA;
}

void DragonComms::calculateChecksum(uint8_t* parseData[]) {
  uint16_t sum = 0;
  for (int i = 0; i < 4; i++) {
    sum += parseData[i];
  }
  parseData[4] = sum & 0xFF;         // First byte of checksum
  parseData[5] = (sum >> 8) & 0xFF;  // Second byte of checksum
}


void DragonComms::sendData(uint8_t* sData[]) {
  calculateChecksum(sData);
  // Send data to each receiver
  esp_now_send(_broadcastAddress, sData, sizeof(sData));
  // Repeat for other receivers...
}

