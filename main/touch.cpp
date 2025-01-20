#include "touch.h"

#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>

SPIClass SPI2(HSPI);
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

unsigned long lastReceivedTime = 0;
unsigned long startTouchTime = 0;
unsigned long lastTouchTime = 0;
const byte address[6] = "00001";

void touch_init() {
  SPI2.begin(D13, D12, D11);
  radio.begin(NRF_CE_PIN, NRF_CSN_PIN);  // initialise NRF24L01
  radio.setChannel(5);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void touch_update() {
  if(!radio.available()) {
    lastReceivedTime = millis();
    float receivedData[] = {0,0};
    radio.read(receivedData, sizeof(receivedData));

    // Debug: Print received data
    /*
    Serial.print("Touch Data: ");
    Serial.print(receivedData[0]);
    Serial.print(", ");
    Serial.println(receivedData[1]);
    */
  }
}
