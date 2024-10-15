#ifndef SYNCHRONISATION_H
#define SYNCHRONISATION_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

class DragonComms {
  private:
    uint8_t _broadcastAddress[6]; 

    void calculateChecksum(uint8_t* parseData);

  public:
    DragonComms(uint8_t* bcA);
    void sendData(uint8_t* sData, size_t dataSize);
};

#endif
